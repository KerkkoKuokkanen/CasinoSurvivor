
#include "objSelector.h"
#include "imgui.h"
#include <optional>
#include <set>
#include "envHandler.h"
#include <math.h>
#include "commonTools.h"
#include "roomLoading.h"
#include "copyObject.h"

// Unique ID generator for nodes
static int next_id = 1;
unsigned int objCounter = 0;
bool changeHappened = false;

// Structure to represent folders and game objects
struct Node {
	int id;
	std::string name;
	bool is_folder;
	std::optional<int> parent_id;
	bool renaming = false;
	char rename_buffer[128] = "";
	uint64_t objKey;
	uint16_t room;

	Node(std::string n, bool folder, std::optional<int> parent = std::nullopt)
		: id(next_id++), name(std::move(n)), is_folder(folder), parent_id(parent) {
		std::strncpy(rename_buffer, name.c_str(), sizeof(rename_buffer));
		room = GetCurrentRoom();
	}
};

// Global list of objects and folders
std::vector<Node> nodes;
std::optional<int> selected_node_id = std::nullopt;
std::set<int> nodes_to_delete; // Set of nodes to delete after loop

// Find a node by ID
Node* FindNodeById(int id) {
	for (auto& node : nodes) {
		if (node.id == id)
			return &node;
	}
	return nullptr;
}

Node* FindNodeByObjKey(uint64_t key) {
	for (auto& node : nodes) {
		if (node.objKey == key)
			return &node;
	}
	return nullptr;
}

void MarkChildrenForDeletion(int parent_id) {
	for (auto& node : nodes) {
		if (node.parent_id.has_value() && *node.parent_id == parent_id) {
			nodes_to_delete.insert(node.id); // Mark child for deletion
			MarkChildrenForDeletion(node.id); // Recursively delete children
		}
	}
}

void ProcessDeletions() {
	if (!nodes_to_delete.empty()) {
		changeHappened = true;
		// Step 1: Recursively mark children of deleted folders
		std::set<int> nodes_to_process = nodes_to_delete;
		for (int id : nodes_to_process) {
			MarkChildrenForDeletion(id); // Find and mark children
		}

		// Step 2: Destroy all SystemObj instances before deletion
		for (int id : nodes_to_delete) {
			Node* node = FindNodeById(id);
			if (node) {
				SystemObj* obj = FindSystemObject(node->objKey);
				if (obj != NULL) {
					obj->Destroy(); // ✅ Ensure destruction of game objects
				}
			}
		}

		// Step 3: Actually remove the nodes from the list
		nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
			[](const Node& node) {
				return nodes_to_delete.count(node.id) > 0;
			}),
			nodes.end());

		// Step 4: Clear the deletion tracking set
		nodes_to_delete.clear();
	}
}


// Handles renaming logic
void RenderRenameInput(Node& node) {
	ImGui::SetKeyboardFocusHere();
	if (ImGui::InputText("##rename", node.rename_buffer, sizeof(node.rename_buffer),
						ImGuiInputTextFlags_EnterReturnsTrue)) {
		node.name = node.rename_buffer;
		node.renaming = false;
		changeHappened = true;
	}
	if (!ImGui::IsItemActive() && ImGui::IsMouseClicked(0)) {
		node.renaming = false;
	}
}

// Render the object hierarchy recursively
void ShowHierarchy(std::optional<int> parent_id = std::nullopt) {
	for (auto& node : nodes) {
		if (node.parent_id != parent_id)
			continue;

		bool selected = (selected_node_id && *selected_node_id == node.id);

		if (node.is_folder) {
			bool open = ImGui::TreeNodeEx(node.name.c_str(), selected ? ImGuiTreeNodeFlags_Selected : 0);

			if (node.renaming) {
				RenderRenameInput(node);
			}

			// Right-click menu
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Rename")) {
					node.renaming = true;
				}
				if (ImGui::MenuItem("Delete")) {
					nodes_to_delete.insert(node.id); // Mark for deletion
				}
				if (ImGui::MenuItem("Create Folder")) {
					std::string used = "New Folder" + std::to_string(next_id);
					nodes.emplace_back(used.c_str(), true, node.id);
				}
				if (ImGui::MenuItem("Create Object")) {
					SystemObj *added = new SystemObj();
					std::string used = "New Object" + std::to_string(next_id);
					nodes.emplace_back(used.c_str(), false, node.id);
					nodes[nodes.size() - 1].objKey = added->GetSystemObjectKey();
				}
				ImGui::EndPopup();
			}

			if (ImGui::IsItemClicked()) selected_node_id = node.id;

			// Drag & Drop source
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				ImGui::SetDragDropPayload("NODE_PAYLOAD", &node.id, sizeof(int));
				ImGui::Text("Moving: %s", node.name.c_str());
				ImGui::EndDragDropSource();
			}

			// Drag & Drop target
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE_PAYLOAD")) {
					int dragged_id = *(int*)payload->Data;
					if (dragged_id != node.id) {
						if (Node* dragged_node = FindNodeById(dragged_id)) {
							dragged_node->parent_id = node.id;
						}
					}
					changeHappened = true;
				}
				ImGui::EndDragDropTarget();
			}

			if (open) {
				ShowHierarchy(node.id);
				ImGui::TreePop();
			}
		} else {
			// Render Game Objects
			if (node.renaming) {
				RenderRenameInput(node);
			} else {
				if (ImGui::Selectable(node.name.c_str(), selected))
					selected_node_id = node.id;
			}

			// Right-click menu
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Rename")) node.renaming = true;
				if (ImGui::MenuItem("Delete")) {
					nodes_to_delete.insert(node.id); // Mark for deletion
				}
				if (!node.is_folder) { // Only for actual objects, not folders
					if (ImGui::MenuItem("Copy Key")) {
						char key_str[32];  // Buffer for the key
						snprintf(key_str, sizeof(key_str), "%llu", node.objKey); // Convert uint64_t to string
						ImGui::SetClipboardText(key_str); // ✅ Copy to clipboard
					}
					if (ImGui::MenuItem("Duplicate")) {
						SystemObj *dup = FindSystemObject(node.objKey);
						if (dup != NULL)
							CopyObject(dup);
					}
				}
				ImGui::EndPopup();
			}

			// Drag & Drop source
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				ImGui::SetDragDropPayload("NODE_PAYLOAD", &node.id, sizeof(int));
				ImGui::Text("Moving: %s", node.name.c_str());
				ImGui::EndDragDropSource();
			}

			// Drag & Drop target
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE_PAYLOAD")) {
					int dragged_id = *(int*)payload->Data;
					if (dragged_id != node.id) {
						if (Node* dragged_node = FindNodeById(dragged_id)) {
							dragged_node->parent_id = node.parent_id;
						}
					}
					changeHappened = true;
				}
				ImGui::EndDragDropTarget();
			}
		}
	}
}

static bool hovered = false;

bool GetObjSelectorHover()
{
	return (hovered);
}

// UI Function to show the hierarchy window
void ShowHierarchyWindow() {
	ImGui::Begin("Object Hierarchy");
	hovered = ImGui::IsWindowHovered();

	// Root-level actions
	if (ImGui::Button("Create Folder")) {
		std::string used = "New Folder" + std::to_string(next_id);
		nodes.emplace_back(used.c_str(), true);
		changeHappened = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Create Object")) {
		SystemObj *added = new SystemObj();
		std::string used = "New Object" + std::to_string(next_id);
		nodes.emplace_back(used.c_str(), false);
		nodes[nodes.size() - 1].objKey = added->GetSystemObjectKey();
		changeHappened = true;
	}

	// Dragging items back to root
	ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, 6)); // Or any height
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE_PAYLOAD")) {
			int dragged_id = *(int*)payload->Data;
			if (Node* dragged_node = FindNodeById(dragged_id)) {
				dragged_node->parent_id = std::nullopt; // Move back to root
			}
			changeHappened = true;
		}
		ImGui::EndDragDropTarget();
	}

	ShowHierarchy();

	ProcessDeletions(); // Delete marked nodes **AFTER** rendering

	ImGui::End();
}

void PutObjsInNodes(std::unordered_map<uint64_t, SystemObj*> &objs, uint64_t self)
{
	for (auto obj : objs)
	{
		if (obj.first == self)
			continue ;
		if (FindNodeByObjKey(obj.second->GetSystemObjectKey()) == nullptr)
		{
			if (obj.second->GetComponent("EngineHierarchy") != NULL)
				continue ;
			changeHappened = true;
			std::string used = "New Object" + std::to_string(next_id);
			nodes.emplace_back(used.c_str(), false);
			nodes[nodes.size() - 1].objKey = obj.second->GetSystemObjectKey();
			nodes[nodes.size() - 1].room = obj.second->GetSaveableRoom();
		}
	}
}

void ObjectSelector::InitSecondaryHierarchy(void *sec)
{
	EngineHierarchy *second = (EngineHierarchy*)sec;
	if (second->self->GetSaveableRoom() == GetCurrentRoom())
	{
		if (hieararchy != NULL)
			return ;
		init = true;
		hieararchy = second;
	}
	else
		secondaryHieararchies[second->self->GetSaveableRoom()] = second;
	if (second->currentData.size() == 0)
		return ;
	std::vector<NodeData> &data = second->currentData;
	for (NodeData n : data)
	{
		if (n.parent_id != (-1))
			nodes.emplace_back(n.name, n.is_folder, n.parent_id);
		else
			nodes.emplace_back(n.name, n.is_folder);
		nodes[nodes.size() - 1].objKey = n.objKey;
		nodes[nodes.size() - 1].room = second->self->GetSaveableRoom();
	}
}

void ObjectSelector::InitSelector()
{
	if (init)
		return ;
	init = true;
	if (hieararchy->currentData.size() == 0)
		return ;
	std::vector<NodeData> &data = hieararchy->currentData;
	for (NodeData n : data)
	{
		if (n.parent_id != (-1))
			nodes.emplace_back(n.name, n.is_folder, n.parent_id);
		else
			nodes.emplace_back(n.name, n.is_folder);
		nodes[nodes.size() - 1].objKey = n.objKey;
		nodes[nodes.size() - 1].room = hieararchy->self->GetSaveableRoom();
	}
}

void ObjectSelector::SetHierarchy()
{
	if (hieararchy != NULL || init)
		return ;
	hieararchy = (EngineHierarchy*)FindAny("EngineHierarchy");
	if (hieararchy == NULL)
	{
		SystemObj *obj = new SystemObj();
		hieararchy = (EngineHierarchy*)obj->AddComponent("EngineHierarchy");
	}
}

void ObjectSelector::SaveNodesData()
{
	if (changeHappened == false)
		return ;
	std::vector<NodeData> data = {};
	std::unordered_map<uint16_t, std::vector<NodeData>> datas;
	uint16_t currRoom = GetCurrentRoom();
	for (Node n : nodes)
	{
		NodeData add;
		std::strncpy(add.name, n.name.c_str(), sizeof(add.name) - 1);
		add.name[sizeof(add.name) - 1] = '\0';
		add.is_folder = n.is_folder;
		if (n.parent_id.has_value())
			add.parent_id = n.parent_id.value();
		else
			add.parent_id = -1;
		add.objKey = n.objKey;
		if (currRoom != n.room)
			datas[n.room].push_back(add);
		else
			data.push_back(add);
	}
	if (hieararchy != NULL)
		hieararchy->SaveHierarchy(data);
	for (auto &[key, vec] : datas)
		secondaryHieararchies[key]->SaveHierarchy(vec);
}

ObjectSelector::~ObjectSelector()
{
	nodes.clear();
	selected_node_id = std::nullopt;
	nodes_to_delete.clear();
	next_id = 1;
	objCounter = 0;
	changeHappened = false;
}

void ObjectSelector::SaveRoomChange(uint64_t objId, uint16_t room)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].objKey == objId)
		{
			nodes[i].room = room;
			changeHappened = true;
			SaveNodesData();
			break ;
		}
	}
}

std::tuple<uint64_t, bool, std::string> ObjectSelector::UpdateObjectSelector(std::unordered_map<uint64_t, SystemObj*> &objs, uint64_t self)
{
	changeHappened = false;
	SetHierarchy();
	InitSelector();
	PutObjsInNodes(objs, self);
	ShowHierarchyWindow();
	SaveNodesData();
	if (!selected_node_id.has_value())
		return (std::tuple<uint64_t, bool, std::string>{0, false, ""});
	int id = *selected_node_id;
	Node *node = FindNodeById(id);
	if (node == nullptr)
		return (std::tuple<uint64_t, bool, std::string>{0, false, ""});
	return (std::tuple<uint64_t, bool, std::string>{node->objKey, true, node->name});
}

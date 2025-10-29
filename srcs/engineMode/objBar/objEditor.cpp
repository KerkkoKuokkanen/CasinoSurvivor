
#include "objEditor.h"
#include "imgui.h"
#include "customComponent.h"
#include "image.h"
#include "roomLoading.h"
#include "envHandler.h"
#include "objBar.h"

void ObjectEditor::SecureDeleteButton(SystemObj *obj)
{
	static double holdStartTime = 0.0;
	static bool isHolding = false;
	const double holdDuration = 0.5; // Require 0.5 seconds hold

	// Render button (but don't use it for activation)
	ImGui::Button("Delet Component##Hold", ImVec2(120, 20));

	// Check if button is currently being held down
	if (ImGui::IsItemActive()) 
	{
		if (!isHolding) 
		{
			holdStartTime = ImGui::GetTime(); // Start timer only once
			isHolding = true;
		}

		double elapsed = ImGui::GetTime() - holdStartTime;
		float progress = elapsed / holdDuration;

		if (elapsed >= holdDuration) // Hold long enough
		{
			if (selectWindow == n_ComponentTypes::IMAGE_CLASS)
			{
				t_sysComponent comp = obj->components[compIndex];
				Image *img = (Image*)comp.obj;
				delete (img);
			}
			else
			{
				t_sysComponent comp = obj->components[compIndex];
				CustomComponent *cust = (CustomComponent*)comp.obj;
				delete (cust);
			}
			selectWindow = 0;
			isHolding = false; // Reset state
		}
	}
	else 
	{
		// Reset if released early
		isHolding = false;
	}
}

void ObjectEditor::ComponentSelector(SystemObj *obj)
{
	ImGui::Text("Components:");
	if (ImGui::Button("Add Component"))
		selectWindow = -1;
	for (int i = 0; i < obj->components.size(); i++)
	{
		t_sysComponent comp = obj->components[i];
		std::string str = comp.type + " " + std::to_string(i);
		if (ImGui::Button(str.c_str()))
		{
			selectWindow = comp.classType;
			compIndex = i;
		}
	}
	ImGui::NewLine();
	ImGui::Text("Layer Manager:");
	if (ImGui::Button("Layer Options"))
		selectWindow = -2;
	if (ImGui::Button("SaveSettings"))
		selectWindow = -3;
}

void ObjectEditor::LayerManager()
{
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	ImGui::Text("Layer Manager");
	ImGui::NewLine();
	const char* options[] = { "No Sort", "Y-Sort", "Depth Sort", "Depth & Y-Sort", "Multi Sprite", "Structure", "Text"};
	static int currentIndex = 0;
	static int layerNumber = 0;

	ImGui::Text("Add New Layer:");
	ImGui::InputInt("Number", &layerNumber, 1, 1);
	int selectedValue = 0;
	if (ImGui::Combo("Type", &currentIndex, options, IM_ARRAYSIZE(options)))
	{
		switch (currentIndex)
		{
			case 0: selectedValue = n_SortTypes::NO_SORT; break;
			case 1: selectedValue = n_SortTypes::Y_SORT; break;
			case 2: selectedValue = n_SortTypes::DEPTH_SORT; break;
			case 3: selectedValue = n_SortTypes::DEPTH_Y_SORT; break;
			case 4: selectedValue = n_SortTypes::TEXT_LAYER; break;
			case 5: selectedValue = n_SortTypes::MULTI_LAYER; break;
			default: selectedValue = -1; break;
		}
	}
	if (ImGui::Button("Add Layer"))
		universalRenderingSystem.AddLayer(layerNumber, currentIndex);

	ImGui::NewLine();
	ImGui::Text("Remove Layer:");
	std::vector<std::tuple<int, int>> data = universalRenderingSystem.GetLayerData();
	static int currentIndex2 = 0;

	std::vector<std::string> items;
	for (const auto& entry : data) {
		items.push_back("Layer: " + std::to_string(std::get<0>(entry)) + 
						"  Type: " + options[std::get<1>(entry)]);
	}

	std::vector<const char*> itemPtrs;
	for (const auto& item : items)
		itemPtrs.push_back(item.c_str());

	if (ImGui::Combo("Layer", &currentIndex2, itemPtrs.data(), itemPtrs.size()))
	{
		int selectedID = std::get<0>(data[currentIndex2]);
		int selectedValue = std::get<1>(data[currentIndex2]);
	}
	bool returnVal = true;
	if (ImGui::Button("Remove"))
		returnVal = universalRenderingSystem.RemoveLayer(std::get<0>(data[currentIndex2]));
	if (returnVal == false)
		printf("Layer contains data or did not exits. Could not remove.\n");
}

void ObjectEditor::TransformDropDown(SystemObj *obj)
{
	t_sysComponent com = obj->components[compIndex];
	Image *img = (Image*)com.obj;
	const char* options[] = { "Camera", "Static"};
	int currentIndex = img->GetTransformType();
	if (currentIndex == n_TransformTypes::TRANSFORM_CAMERA)
		currentIndex = 0;
	else
		currentIndex = 1;

	std::vector<const char*> itemPtrs;
	for (const auto& item : options) {
		itemPtrs.push_back(item);
	}

	if (ImGui::Combo("T", &currentIndex, itemPtrs.data(), itemPtrs.size()))
	{
		int selectedID = currentIndex;
	}
	if (currentIndex == 0)
		img->SetTransformType(n_TransformTypes::TRANSFORM_CAMERA);
	else if (currentIndex == 1)
		img->SetTransformType(n_TransformTypes::TRANSFORM_STATIC);
}

void ObjectEditor::LayerSelectionDropDown(SystemObj *obj)
{
	t_sysComponent com = obj->components[compIndex];
	Image *img = (Image*)com.obj;
	std::vector<std::tuple<int, int>> data = universalRenderingSystem.GetLayerData();
	const char* options[] = { "No Sort", "Y-Sort", "Depth Sort", "Depth & Y-Sort", "Text Layer", "Multi Sprite"};
	int currentIndex = 0;
	for (auto [l, t] : data)
	{
		if (l == img->GetLayer())
			break ;
		currentIndex += 1;
	}

	std::vector<std::string> items;
	for (const auto& entry : data) {
		items.push_back("Layer: " + std::to_string(std::get<0>(entry)) + 
						"  Type: " + options[std::get<1>(entry)]);
	}

	std::vector<const char*> itemPtrs;
	for (const auto& item : items) {
		itemPtrs.push_back(item.c_str());
	}

	if (ImGui::Combo("Layer", &currentIndex, itemPtrs.data(), itemPtrs.size()))
	{
		int selectedID = std::get<0>(data[currentIndex]);
		int selectedValue = std::get<1>(data[currentIndex]);
	}
	img->SetLayer(std::get<0>(data[currentIndex]));
}

void ObjectEditor::UpdateImageClass(SystemObj *obj)
{
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	ImGui::Text("Image:");
	t_sysComponent com = obj->components[compIndex];
	Image *img = (Image*)com.obj;

	t_Point pos = img->position;
	float a = img->angle;
	float w = img->dimentions.x;
	float h = img->dimentions.y;
	int layer = img->GetLayer();
	t_Box c = img->GetColor();
	float depth = img->drawDepth;
	textData tData = img->GetTextureData();

	SetImageTexture(obj);

	ImGui::Text("Position:");
	ImGui::InputFloat("X", &pos.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Y", &pos.y, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Size:");
	ImGui::InputFloat("Width", &w, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Height", &h, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Rotation:");
	ImGui::InputFloat("Angle", &a, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Color:");
	ImGui::InputFloat("Red", &c.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Green", &c.y, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Blue", &c.w, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Alpha", &c.h, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Depth:");
	ImGui::InputFloat("Depth", &depth, 0.01f, 1.0f, "%.2f");

	ImGui::NewLine();
	ImGui::Text("Texture Data:");
	ImGui::InputFloat("Pos X", &tData.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Pos Y", &tData.y, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("t Width", &tData.w, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("t Height", &tData.h, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("t Angle", &tData.a, 0.01f, 1.0f, "%.2f");

	ImGui::NewLine();
	TransformDropDown(obj);

	ImGui::NewLine();
	LayerSelectionDropDown(obj);

	img->position = {pos.x, pos.y};
	img->angle = a;
	img->dimentions.x = w;
	img->dimentions.y = h;
	img->SetColor(c.x, c.y, c.w, c.h);
	img->drawDepth = depth;
	img->SetTextureData(tData.x, tData.y, tData.w, tData.h, tData.a);

	ImGui::NewLine();
	SecureDeleteButton(obj);
}

void ObjectEditor::UpdateSaveSettings(SystemObj *obj)
{
	static int room = 1;
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	ImGui::Text("Saveable:");
	if (ImGui::Button("yes"))
		obj->SetSaveable(true, obj->GetSaveableRoom());
	ImGui::SameLine();
	if (ImGui::Button("no"))
		obj->SetSaveable(false, obj->GetSaveableRoom());

	static char searchBuffer[64] = "";
	static std::string selected = "";

	if (ImGui::Button("Room"))
		ImGui::OpenPopup("LoadPopup2");

	if (ImGui::BeginPopup("LoadPopup2")) {
		ImGui::InputText("s", searchBuffer, IM_ARRAYSIZE(searchBuffer));

		std::vector<std::string> items = {};
		std::vector<uint16_t> loadedRooms = GetLoadedRooms();
		for (uint16_t lr : loadedRooms)
			items.push_back(GetRoomWithKey(lr));
		std::string searchQuery(searchBuffer);

		for (const auto& item : items) {
			if (searchQuery.empty() || item.find(searchQuery) != std::string::npos) {
				if (ImGui::Selectable(item.c_str())) {
					selected = item;
					ImGui::CloseCurrentPopup();
				}
			}
		}

		ImGui::EndPopup();
	}

	if (!selected.empty()) {
		uint16_t room = GetRoomWithName(selected);
		obj->SetSaveable(obj->GetSaveable(), room);
		ObjBar *bar = (ObjBar*)FindAny("ObjBar");
		if (bar != NULL)
			bar->SaveRoomChange(obj->GetSystemObjectKey(), room);
		selected.clear();
	}
}

void ObjectEditor::UpdateSelectedWindow(SystemObj *obj)
{
	switch (selectWindow)
	{
		case -1:
			ComponentAdder(obj);
			break ;
		case -2:
			LayerManager();
			break ;
		case -3:
			UpdateSaveSettings(obj);
			break ;
		case 0:
			ComponentSelector(obj);
			break ;
		case n_ComponentTypes::IMAGE_CLASS:
			UpdateImageClass(obj);
			break ;
		case n_ComponentTypes::STRUCTURE_CLASS:
			break ;
		default:
			UpdateCustomComponent(obj);
			break ;
	}
}

void ObjectEditor::NoSelectWindow()
{
	ImGui::Text("Object Not Selected");
	ImGui::End();
}

bool ObjectEditor::IsHovered()
{
	return (isHovered);
}

void ObjectEditor::UpdateSelected(uint64_t key, bool selected, std::string name)
{
	ImGui::Begin("Object Editor");
	isHovered = ImGui::IsWindowHovered();
	if (selected == false)
		return (NoSelectWindow());
	SystemObj *obj = FindSystemObject(key);
	if (obj == NULL)
		return (NoSelectWindow());
	if (lastSelected != obj->GetSystemObjectKey())
	{
		lastSelected = obj->GetSystemObjectKey();
		compIndex = 0;
		selectWindow = 0;
	}
	name += ":";
	ImGui::Text("%s", name.c_str());
	UpdateSelectedWindow(obj);
	ImGui::End();
}

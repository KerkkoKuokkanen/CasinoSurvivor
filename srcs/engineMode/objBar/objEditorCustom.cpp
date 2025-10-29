
#include "customComponent.h"
#include "componentRegistry.h"
#include "objEditor.h"
#include "imgui.h"
#include <algorithm> 
#include "Textures.h"
#include "image.h"
#include "spriteBatch.h"

int ObjectEditor::LayerDropDown()
{
	std::vector<std::tuple<int, int>> data = universalRenderingSystem.GetLayerData();
	const char* options[] = { "No Sort", "Y-Sort", "Depth Sort", "Depth & Y-Sort", "Text Layer", "Multi Sprite"};
	static int currentIndex = 0;

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
		// Extract selected tuple values
		int selectedID = std::get<0>(data[currentIndex]);
		int selectedValue = std::get<1>(data[currentIndex]);
	}
	return (std::get<0>(data[currentIndex]));
}

uint64_t ObjectEditor::GetTextureHash()
{
	static char searchBuffer[64] = {0}; // Input field
	std::unordered_map<std::string, t_Texture> texts = GetAllTextures();
	std::vector<std::string> items = {};
	for (auto it : texts)
		items.push_back(it.first);
	static std::vector<std::string> filteredItems; // Stores filtered results
	static bool isDropdownOpen = false;

	// Input field
	if (ImGui::InputText("Search", searchBuffer, sizeof(searchBuffer)))
	{
		std::string searchTerm = searchBuffer;
		filteredItems.clear();

		// Convert search term to lowercase
		std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);

		if (!searchTerm.empty())
		{
			isDropdownOpen = true;

			for (const auto& item : items)
			{
				// Convert list item to lowercase for comparison
				std::string lowerItem = item;
				std::transform(lowerItem.begin(), lowerItem.end(), lowerItem.begin(), ::tolower);

				// If the item contains the search term (case insensitive), add it to results
				if (lowerItem.find(searchTerm) != std::string::npos)
				{
					filteredItems.push_back(item); // Store original case version
				}
			}

			// Sort matches by how well they match (closer to start = higher priority)
			std::sort(filteredItems.begin(), filteredItems.end(), [&searchTerm](const std::string& a, const std::string& b) {
				std::string lowerA = a, lowerB = b;
				std::transform(lowerA.begin(), lowerA.end(), lowerA.begin(), ::tolower);
				std::transform(lowerB.begin(), lowerB.end(), lowerB.begin(), ::tolower);
				return lowerA.find(searchTerm) < lowerB.find(searchTerm);
			});
		}
		else
		{
			isDropdownOpen = false;
		}
	}

	// Show dropdown only if matches are found
	if (isDropdownOpen && !filteredItems.empty())
	{
		ImGui::BeginChild("Dropdown", ImVec2(200, 150), true);
		for (const auto& item : filteredItems)
		{
			if (item == "ObjBar" || item == "EngineHierarchy")
				continue ;
			if (ImGui::Selectable(item.c_str()))
			{
				strcpy(searchBuffer, item.c_str()); // Set selected item as input
				isDropdownOpen = false; // Close dropdown
			}
		}
		ImGui::EndChild();
	}
	std::string searched = searchBuffer;
	return (GetTextureGLSign(searched));
}

void ObjectEditor::UpdateMultiSprite(SystemObj *obj)
{
	if (ImGui::Button("<<"))
		selectWindow = 0;
	t_sysComponent comp = obj->components[compIndex];
	ImGui::SameLine();
	ImGui::Text("SpriteBatch");
	SpriteBatch *batch = (SpriteBatch*)comp.obj;
	std::string texture = GetSelectedTexture();
	
	static float width = 1.0f;
	static float height = 1.0f;
	static int maxSize = 1000;
	static int layer = 0;

	ImGui::InputFloat("width", &width, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("height", &height, 0.1f, 1.0f, "%.2f");
	ImGui::InputInt("size", &maxSize);
	ImGui::InputInt("layer", &layer);

	if (ImGui::Button("create"))
		batch->SetUpMultiSprite(texture, width, height, maxSize, layer);

	ImGui::NewLine();

	static t_Point pos = {0.0f, 0.0f};
	static t_Box sRect = {0.0, 0.0f, 1.0f, 1.0f};
	static t_Point dims = {1.0f, 1.0f};
	static float angle = 0.0f;
	static t_Box color = {1.0f, 1.0f, 1.0f, 1.0f};

	ImGui::InputFloat("pos x", &pos.x, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("pos y", &pos.y, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("sRect x", &sRect.x, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("sRect y", &sRect.y, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("sRect w", &sRect.w, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("sRect h", &sRect.h, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("w", &dims.x, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("h", &dims.y, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("angle", &angle, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("color r", &color.x, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("color g", &color.y, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("color b", &color.w, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("color a", &color.h, 0.1f, 1.0f, "%.2f");

	if (ImGui::Button("Add Sprite"))
		batch->AddSprite(pos, sRect, dims, angle, color);

	std::vector<std::tuple<uint32_t, SpriteData>> allSprites = batch->GetAllSprites();
	if (allSprites.size() == 0)
	{
		SecureDeleteButton(obj);
		return ;
	}

	ImGui::NewLine();
	static int currentIndex = 0;
	std::vector<std::string> keyLabels;
	for (const auto& [key, sprite] : allSprites) {
	    keyLabels.push_back(std::to_string(key)); // convert key to string
	}

	// Create a list of const char* for ImGui
	std::vector<const char*> keyLabelCStrs;
	for (const auto& label : keyLabels) {
	    keyLabelCStrs.push_back(label.c_str());
	}

	// Show the dropdown
	if (ImGui::Combo("Sprite Key", &currentIndex, keyLabelCStrs.data(), keyLabelCStrs.size())) {
	
	}
	uint32_t selectedKey = std::get<0>(allSprites[currentIndex]);

	SpriteData currentData = batch->GetSprite(selectedKey);

	ImGui::InputFloat("pos x ", &currentData.pos.x, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("pos y ", &currentData.pos.y, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("sRect x ", &currentData.sRect.x, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("sRect y ", &currentData.sRect.y, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("sRect w ", &currentData.sRect.w, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("sRect h ", &currentData.sRect.h, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("w ", &currentData.dimentions.x, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("h ", &currentData.dimentions.y, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("angle ", &currentData.angle, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("color r ", &currentData.color.x, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("color g ", &currentData.color.y, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("color b ", &currentData.color.w, 0.1f, 1.0f, "%.2f");
	ImGui::InputFloat("color a ", &currentData.color.h, 0.1f, 1.0f, "%.2f");

	batch->ModifySprite(selectedKey, currentData.pos, currentData.sRect, currentData.dimentions, currentData.angle, currentData.color);
	SecureDeleteButton(obj);
}

void ObjectEditor::UpdateCustomComponent(SystemObj *obj)
{
	t_sysComponent comp = obj->components[compIndex];
	if (comp.type == "SpriteBatch")
		return (UpdateMultiSprite(obj));
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	std::string used = comp.type + ".";
	ImGui::Text("%s", used.c_str());
	CustomComponent *cust = (CustomComponent*)comp.obj;
	for (int i = 0; i < cust->inputFields.size(); i++)
	{
		std::string name = std::get<0>(cust->inputFields[i]);
		int varType = std::get<1>(cust->inputFields[i]);
		void *dest = std::get<2>(cust->inputFields[i]);
		switch (varType)
		{
			case n_VarType::INTEGER:
			{
				ImGui::InputInt(name.c_str(), (int*)dest);
				break ;
			}
			case n_VarType::FLOAT:
			{
				ImGui::InputFloat(name.c_str(), (float*)dest, 0.1f);
				break ;
			}
			case n_VarType::TEXT:
			{
				ImGui::InputText(name.c_str(), (char*)dest, sizeof(char) * 32);
				break ;
			}
			case n_VarType::BOOL:
			{
				bool *bol = (bool*)dest;
				if (ImGui::Button(name.c_str()))
					*bol = true;
				else
					*bol = false;
				break ;
			}
			case n_VarType::SPRITES:
			{
				uint64_t *hash = (uint64_t*)dest;
				*hash = GetTextureHash();
				break ;
			}
			case n_VarType::LAYERS:
			{
				int *layer = (int*)dest;
				*layer = LayerDropDown();
				break ;
			}
			default :
			{
				break ;
			}
		}
	}
	SecureDeleteButton(obj);
}

void ObjectEditor::ComponentAdder(SystemObj *obj)
{
	if (ImGui::Button("<<"))
		selectWindow = 0;
	ImGui::SameLine();
	ImGui::Text("Add Component Name");

	static bool showMessage = false;
	static bool goodMessage = false;
	static double startTime = 0.0f;
	const double displayDuration = 2.0;

	static char searchBuffer[64] = {0}; // Input field
	std::vector<std::string> items = GetAllComponentNames();
	items.push_back("image");
	static std::vector<std::string> filteredItems; // Stores filtered results
	static bool isDropdownOpen = false;

	// Input field
	if (ImGui::InputText("Search", searchBuffer, sizeof(searchBuffer)))
	{
		std::string searchTerm = searchBuffer;
		filteredItems.clear();

		// Convert search term to lowercase
		std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);

		if (!searchTerm.empty())
		{
			isDropdownOpen = true;

			for (const auto& item : items)
			{
				// Convert list item to lowercase for comparison
				std::string lowerItem = item;
				std::transform(lowerItem.begin(), lowerItem.end(), lowerItem.begin(), ::tolower);

				// If the item contains the search term (case insensitive), add it to results
				if (lowerItem.find(searchTerm) != std::string::npos)
				{
					filteredItems.push_back(item); // Store original case version
				}
			}

			// Sort matches by how well they match (closer to start = higher priority)
			std::sort(filteredItems.begin(), filteredItems.end(), [&searchTerm](const std::string& a, const std::string& b) {
				std::string lowerA = a, lowerB = b;
				std::transform(lowerA.begin(), lowerA.end(), lowerA.begin(), ::tolower);
				std::transform(lowerB.begin(), lowerB.end(), lowerB.begin(), ::tolower);
				return lowerA.find(searchTerm) < lowerB.find(searchTerm);
			});
		}
		else
		{
			isDropdownOpen = false;
		}
	}

	// Show dropdown only if matches are found
	if (isDropdownOpen && !filteredItems.empty())
	{
		ImGui::BeginChild("Dropdown", ImVec2(200, 150), true);
		for (const auto& item : filteredItems)
		{
			if (item == "ObjBar" || item == "EngineHierarchy")
				continue ;
			if (ImGui::Selectable(item.c_str()))
			{
				strcpy(searchBuffer, item.c_str()); // Set selected item as input
				isDropdownOpen = false; // Close dropdown
			}
		}
		ImGui::EndChild();
	}

	if (ImGui::Button("Add This Component"))
	{
		showMessage = true;
		startTime = ImGui::GetTime();
		std::string compName = searchBuffer;
		void *created = obj->AddComponent(compName);
		if (created == NULL)
			goodMessage = false;
		else
			goodMessage = true;
	}

	if (showMessage)
	{
		double elapsedTime = ImGui::GetTime() - startTime;
		if (elapsedTime < displayDuration)
		{
			if (goodMessage)
				ImGui::Text("Component Created.");
			else
				ImGui::Text("Failed To Create Component.");
		}
		else
			showMessage = false;
	}
}

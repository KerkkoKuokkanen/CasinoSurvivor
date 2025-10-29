
#include "objEditor.h"
#include "Textures.h"
#include "imgui.h"
#include "image.h"

void ObjectEditor::SetImageTexture(SystemObj *obj)
{
	ImGui::Text("Texture:");
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
	if (ImGui::Button("Set Texure"))
	{
		t_sysComponent com = obj->components[compIndex];
		Image *img = (Image*)com.obj;
		img->SetTexture(searched);
	}
	ImGui::NewLine();
}

std::string ObjectEditor::GetSelectedTexture()
{
	ImGui::Text("Texture:");
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
	return (searched);
}

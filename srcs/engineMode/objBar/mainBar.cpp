
#include "mainBar.h"
#include "imgui.h"
#include "envHandler.h"
#include "roomLoading.h"
#include "objBar.h"

bool MainBar::IsHovered()
{
	return (hovered);
}

void MainBar::CreateWindow()
{
	static bool show_popup = false;
	static char name_input[128] = "";

	ImGui::SameLine();

	if (ImGui::Button("Create"))
	{
		show_popup = true;
	}

	if (show_popup)
	{
		ImGui::OpenPopup("Enter Name");

		if (ImGui::BeginPopup("Enter Name"))
		{
			ImGui::Text("Room Name:");

			ImGui::InputText("Name", name_input, sizeof(name_input));

			if (ImGui::Button("Create"))
			{
				std::string createdRoom(name_input);
				CreateNewRoom(createdRoom);
				show_popup = false;
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				show_popup = false;
			}

			ImGui::EndPopup();
		}
	}
}

void MainBar::LoadRoomMBar()
{
	static char searchBuffer[64] = "";
	static std::string selected = "";

	if (ImGui::Button("Load"))
		ImGui::OpenPopup("LoadPopup");

	if (ImGui::BeginPopup("LoadPopup")) {
		ImGui::InputText("s", searchBuffer, IM_ARRAYSIZE(searchBuffer));

		std::vector<std::string> items = GetAllRooms();
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
		LoadRoom(room);
		selected.clear(); // Clear after loading
	}
}

void MainBar::RoomSwitchMBar()
{
	static char searchBuffer[64] = "";
	static std::string selected = "";

	if (ImGui::Button("Room"))
		ImGui::OpenPopup("LoadPopup2");

	if (ImGui::BeginPopup("LoadPopup2")) {
		ImGui::InputText("s", searchBuffer, IM_ARRAYSIZE(searchBuffer));

		std::vector<std::string> items = GetAllRooms();
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
		RoomSwitch(room, {0});
		selected.clear(); // Clear after loading
	}
}

void MainBar::UpdateMainTools()
{
	ImGui::Begin("Engine 2E");
	ImGui::Text("%s\n", GetRoomWithKey(GetCurrentRoom()).c_str());
	hovered = ImGui::IsWindowHovered();

	if (ImGui::Button("Play"))
	{
		ChangeEngineMode(false);
		RoomSwitch(GetCurrentRoom());
	}

	ImGui::SameLine();

	if (ImGui::Button("Save"))
		FullSave();

	ImGui::SameLine();

	LoadRoomMBar();

	ImGui::SameLine();

	RoomSwitchMBar();

	ImGui::SameLine();

	CreateWindow();

	ImGui::End();
}

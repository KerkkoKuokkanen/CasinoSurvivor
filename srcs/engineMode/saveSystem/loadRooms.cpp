
#include <iostream>
#include <filesystem>
#include <string>
#include "saveInterface.h"
#include "commonTools.h"
#include "envHandler.h"
#include <thread>
#include <mutex>

#define WAIT_TIME 10000

namespace fs = std::filesystem;

std::mutex roomMutex;

uint16_t roomKey = 1;
std::unordered_map<uint16_t, std::string> roomsWithKeys;
std::unordered_map<std::string, uint16_t> keysWithRooms;

std::string GetRoomWithKey(uint16_t key)
{
	std::lock_guard<std::mutex> guard(roomMutex);
	if (roomsWithKeys.find(key) == roomsWithKeys.end())
		return ("");
	return (roomsWithKeys[key]);
}

uint16_t GetRoomWithName(std::string name)
{
	std::lock_guard<std::mutex> guard(roomMutex);
	if (keysWithRooms.find(name) == keysWithRooms.end())
		return (0);
	uint16_t key = keysWithRooms[name];
	return (key);
}

std::vector<std::string> GetAllRooms()
{
	std::lock_guard<std::mutex> guard(roomMutex);
	std::vector<std::string> ret = {};
	for (auto &[name, key] : keysWithRooms)
	{
		if (key != 0)
			ret.push_back(name);
	}
	return (ret);
}

bool LoadRoomObjects(uint16_t room)
{
	uint32_t key = SetAskedData(GetRoomWithKey(room) + "/data");
	void *data = NULL;
	for (int i = 0; i < WAIT_TIME; i++)
	{
		bool checker = CollectAskedState(key, &data);
		if (checker)
			break ;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	if (data == NULL)
		return (false);
	SnapShot snap = MakeIntoSnapshot(data);
	LoadObjectsToEnvironment(snap, room);
	free(data);
	return (true);
}

void AddNewRoom(std::string name)
{
	std::lock_guard<std::mutex> guard(roomMutex);
	std::string filePath = "assets/saves/rooms/" + name;
	roomsWithKeys[roomKey] = filePath;
	keysWithRooms[name] = roomKey;
	roomKey += 1;
}

void InitLoadRooms()
{
	std::lock_guard<std::mutex> guard(roomMutex);
	for (const auto& entry : fs::recursive_directory_iterator("assets/saves/rooms"))
	{
		if (fs::is_directory(entry.path()))
		{
			std::string filepath = entry.path().string();
			std::string dirName = entry.path().stem().string(); 
			if (entry.path().extension() != ".rf")
			{
				if (dirName == "engineRoom")
				{
					roomsWithKeys[0] = filepath;
					keysWithRooms[dirName] = 0;
				}
				else
				{
					roomsWithKeys[roomKey] = filepath;
					keysWithRooms[dirName] = roomKey;
					roomKey += 1;
				}
			}
		}
	}
}

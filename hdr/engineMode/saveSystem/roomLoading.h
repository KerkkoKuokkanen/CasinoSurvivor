
#ifndef ROOM_LOADING_H
# define ROOM_LOADING_H

std::string GetRoomWithKey(uint16_t key);
uint16_t GetRoomWithName(std::string name);
bool LoadRoomObjects(uint16_t room);
void AddNewRoom(std::string name);
void InitLoadRooms();
std::vector<std::string> GetAllRooms();

#endif

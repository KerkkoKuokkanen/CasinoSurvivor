
#ifndef ENV_HANDLER_H
# define ENV_HANDLER_H

# include "sysEnv.h"

void ClearSysEnv();
bool LoadEngineRoom();
std::vector<uint16_t> GetLoadedRooms();
bool LoadRoom(uint16_t room);
void DeLoadRoom(uint16_t room);
void RoomSwitch(uint16_t room, std::vector<uint16_t> loaded = {});

bool TakeSnapShot();
bool QuickSave();
bool FullSave();
bool GetControlZ();

SysEnv *GetCurrentEnvironment();
uint16_t GetCurrentRoom();
void SetCurrentRoom(uint16_t room);

void UpdateSysEnv();
void DestroyObject(SystemObj *obj);
void DestroyObject(uint64_t key);
void LoadObjectsToEnvironment(SnapShot snap, uint16_t room);

std::unordered_map<uint64_t, SystemObj*> &GetSysEnvData();
SystemObj *FindSystemObject(uint64_t objKey);
void *FindAny(std::string component);

void ComponentRemover(uint64_t key, uint32_t id);
void CreateNewRoom(std::string name);

#endif

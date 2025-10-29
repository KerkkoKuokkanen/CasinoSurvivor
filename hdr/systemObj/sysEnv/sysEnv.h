
#ifndef SYS_ENV_H
# define SYS_ENV_H

# include "systemObj.h"
# include "sysSaver.h"
# include <unordered_map>

class SysEnv;
static SysEnv* ProtecterCreateSysEnv();

//SysEnv is the class that handles the current rooms custom components
//This is as high as it goes and the only thing after this is the hardcoded
//interface for interaction between new SysEnvs and program start

class SysEnv
{
	private:
		SystemSaver *envState;
		std::vector<SystemObj*> deleting = {};
		std::vector<std::tuple<uint64_t, uint32_t>> compDeleting = {};
		std::unordered_map<uint64_t, SystemObj*> envSysObjs;
		std::vector<uint16_t> deLoadedRooms = {};
		void SnapLoading(sysKeyObj keyObj);
		SysEnv();
		friend SysEnv* ProtecterCreateSysEnv();
	public:
		~SysEnv();
		std::unordered_map<uint64_t, SystemObj*> &GetEnvSysObjData() {return(envSysObjs);};
		bool DeleteObject(uint64_t key);
		void RemoveObject(SystemObj *remove);
		void AddObject(SystemObj *add);
		void AddRoomDeLoading(uint16_t room) {deLoadedRooms.push_back(room);};
		bool SaveState();
		void LoadObjects(SnapShot &snap, uint16_t room);
		void Clear();
		SystemObj *FindObject(uint64_t key);
		void AddToDeleting(SystemObj *deleted) {deleting.push_back(deleted);};
		void UpdateSysObjects();
		void LastUpdateSysObjects();
		void ComponentRemove(uint64_t key, uint32_t id) {compDeleting.push_back({key, id});};
		void *FindAny(std::string component);
};

bool EngineModeOn();
void ChangeEngineMode(bool change);

bool OverImgui();
void ChangeOverImgui(bool change);

#endif

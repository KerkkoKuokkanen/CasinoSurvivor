
#ifndef SYSTEM_OBJ_H
# define SYSTEM_OBJ_H

# include <vector>
# include <iostream>

# define IMAGE_COMPONENT "image"
# define STRUCTURE_COMPONENT "structure"

namespace n_ComponentTypes
{
	//Keep the image classes first
	enum {
		NO_CLASS,
		STRUCTURE_CLASS,
		IMAGE_CLASS,
		CUSTOM_CLASS
	};
};

typedef struct s_sysComponent
{
	uint32_t uniqueKey;
	uint32_t classType;
	bool started;
	std::string type;
	void *obj;
}				t_sysComponent;

//SystemObj is the class that is used for modular work in the engine
//The class is automatically managed by SysEnv class
//It offers you management of N amount of components and transformation in one place
//They have uniquekeys that are saved between sessions that you can use for accessing different systemObjects

class SystemObj
{
	private:
		bool deleting = false;
		int componentSaveFetchIndex = 0;
		uint64_t uniqueSystemObjKey = 0;
		uint32_t saveable = 0;
		void ResetComponentSaveFetching() {componentSaveFetchIndex = 0;};
		void IncrimentComponentFetching() {componentSaveFetchIndex += 1;};
		void *FetchComponentSaveData(void *buffer, size_t buffSize, size_t &compSize);
		size_t FetchComponentDataSize();
		uint32_t FetchComponentClassType();
		uint32_t FetchComponentUniqueKey();
		bool ComponentFetchingAtEnd();
		void DeleteComponentOwn(void *component, uint32_t classType);
		void GiveComponentId(void *component, uint32_t classType, uint32_t id);
		void SetUniqueKeyManual(uint64_t key) {uniqueSystemObjKey = key;};
		void AddObjectController(void *controller);
		void UpdateSystemObj();
		void LastUpdateSystemObj();
		void *AddImageComponent();
		void *AddComponent(void *component, uint32_t classType);
		void *AddComponent(const std::string component, void *initData, size_t initDataSize);
		friend class SystemSaver;
		friend class SysEnv;
		friend void UpdateSysEnv();
		friend SystemObj *CopyObject(SystemObj *source);
		~SystemObj();
	public:
		bool active = true;
		void *controller = NULL;
		float weight = 0.0f;
		std::vector<t_sysComponent> components = {};

		SystemObj();
		bool GetDeleting() {return (deleting);};
		bool GetSaveable();
		uint16_t GetSaveableRoom();
		void SetSaveable(uint8_t save, uint16_t room = 0);
		void ClearSaveable() {saveable = 0;};
		uint64_t GetSystemObjectKey() {return (uniqueSystemObjKey);};
		uint64_t SystemObjectKey() {return (uniqueSystemObjKey);};
		void RemoveComponent(uint32_t id);
		void *GetComponent(const std::string &component);
		std::vector<void*> GetComponents(const std::string &components);
		void *AddComponent(const std::string component);
		void *AddComponent(void *component, const std::string name);
		void ReferenceObject(std::string name, uint64_t key);
		void Destroy();
};

#endif

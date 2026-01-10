
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
		bool active = true;			//Active for if the components are updated inside the object
		void *controller = NULL;	//Controller is the environment, top of the chain
		float weight = 0.0f;		//lower weight objects are updated first
		std::vector<t_sysComponent> components = {};		//Components of the object

		SystemObj();			//Constructor, automatically assigns object to environment
		bool GetDeleting() {return (deleting);};		//Helper function for internal stuff
		bool GetSaveable();								//Helper function for internal stuff, Tells if the object is added to save file
		uint16_t GetSaveableRoom();						//Gets info for what room the object is saved to

		//Sets the save room manually for the object
		//room 0 is default for the current room
		//0 is by deafault the engine room and bypass == true lets you use the 0 value for engine room
		void SetSaveable(uint8_t save, uint16_t room = 0, bool bypass = false);
		void ClearSaveable() {saveable = 0;};	//Makes object not saveable anymore
		uint64_t GetSystemObjectKey() {return (uniqueSystemObjKey);};	//Get the key for the object
		uint64_t SystemObjectKey() {return (uniqueSystemObjKey);};		//Get the key for the object

		//Removes the component from SystemObject without deleting it
		void PopComponent(uint32_t id);

		//Remove the component of the given id
		//If you want to remove a component use the RemoveSelf method of the CustomComponent
		//This method also works but should not be called multiple times in the same frame for the same component
		//Custom component uses this for the removal but also keeps track that it is only called once for itself
		void RemoveComponent(uint32_t id, bool destructed = false);
		void *GetComponent(const std::string &component);	//Get the first component matching the name or NULL
		std::vector<void*> GetComponents(const std::string &components);	//Get all the components matching the name
		void *AddComponent(const std::string component);				//Add a new component with the component name
		void *AddComponent(void *component, const std::string name);	//Add already built component with the name
		void Destroy();		//Destroy the system object
};

#endif

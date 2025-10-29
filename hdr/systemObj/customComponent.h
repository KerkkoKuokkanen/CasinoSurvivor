
#ifndef CUSTOM_COMPONENT_H
# define CUSTOM_COMPONENT_H

# include "systemObj.h"

# define DEFAULT_SAVE_SIZE 256
# define READ_AND_ADVANCE(ptr, type) (*reinterpret_cast<type*>(ptr)); ptr += sizeof(type);

typedef std::tuple<void*, size_t, uint64_t> tracking;

namespace n_VarType
{
	enum {
		INTEGER,
		FLOAT,
		TEXT,
		BOOL,
		SPRITES,
		LAYERS
	};
};

//Custom component is the class that most scripts should inherit from
//SystemObj automatically updates and takes care of CustomComponents memory
//The class offers all the basic functions you need and the possibility for saving the component

//In the Init function void* the tracked variables are returned first and then the memory chunks

class CustomComponent
{
	private:
		void *initData = NULL;
		size_t initDataSize = 0;
		std::vector<tracking> saveTracking = {};
		std::vector<std::tuple<void*, size_t>> saveTrack = {};
		std::vector<std::tuple<std::string, int, void*>> inputFields;
		friend class ObjectEditor;
	protected:
		void AddToSave(void *addition, size_t addSize);
		void RemoveFromSave(void *removed, size_t size);
		void ClearSaveData();
		void ClearToSave();
		void CreateInputField(std::string name, int varType, void *dest);
		void AddToSaveTracking(void *addition, size_t size);
	public:
		uint32_t ownId = 0;
		SystemObj *self = NULL;
		virtual ~CustomComponent();
		size_t GetComponentSize();
		virtual void *CollectSaveData(size_t &size);

		//Init is called right after the component is created
		//Should not rely on any other components or try to get components
		virtual void Init(void *data, size_t size) {};

		//Start is called once and guanranteed to happen before the first update
		//Will be called after each component of SystemObj *self has performed the Init function
		//In the Init function void* the tracked variables are returned first and then the memory chunks
		virtual void Start() {};

		//Update: self explainatory
		virtual void Update() {};

		//Update function that is called after updates and rendering
		virtual void LastUpdate() {};

		//Update function that is called at the same time as LastUpdate()
		//Is only called during engine mode is on, while other updates are not.
		virtual void EngineUpdate() {};

		//This function is called right before the object gets deleted
		virtual void Destroy() {};
};

#endif


#ifndef CUSTOM_COMPONENT_H
# define CUSTOM_COMPONENT_H

# include "systemObj.h"
# include "deltaTime.h"
# include "commonTools.h"

# define DEFAULT_SAVE_SIZE 256
# define READ_AND_ADVANCE(ptr, type) (*reinterpret_cast<type*>(ptr)); ptr += sizeof(type);

typedef std::tuple<void*, size_t, uint64_t> tracking;

//Variable types for inputfields
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

struct InvokeStruct{
	std::function<void()> f;
	int loops;
	float repeatTime;
	float counter;
};

//Custom component is the class that most scripts should inherit from
//SystemObj automatically updates and takes care of CustomComponents memory
//The class offers all the basic functions you need and the possibility for saving the component

//In the Init function void* the tracked variables are returned first and then the memory chunks

class CustomComponent
{
	private:
		bool deleted = false;
		void *initData = NULL;
		size_t initDataSize = 0;
		std::vector<tracking> saveTracking = {};
		std::vector<std::tuple<void*, size_t>> saveTrack = {};
		std::vector<std::tuple<std::string, int, void*>> inputFields;
		std::vector<InvokeStruct> invokes;
		void UpdateInvokes();
		friend void UpdateCustInvokes(CustomComponent *cust);
		friend class ObjectEditor;
	protected:
		void AddToSave(void *addition, size_t addSize);		//Add a block of data for saving
		void RemoveFromSave(void *removed, size_t size);	//Remove the block of data from saving
		void ClearSaveData();					//Clears added data blocks
		void ClearSaveTracking();				//Clears tracked variables
		void CreateInputField(std::string name, int varType, void *dest);		//Creates an inputfield in the engine
		void AddToSaveTracking(void *addition, size_t size);					//Adds variable to be tracked and saved
		//Automatically calls the given function every repeat time
		void Invoke(std::function<void()> f, int loops, float repeatTime) {invokes.push_back({f, loops, repeatTime, 0.0f});};

		//Removes itself
		//Use this insted of the remove component with systemObj to be safer
		void RemoveSelf();
	public:
		bool active = true;						//active: true -> run update, false -> no update and no invokes
		float componentWeight = 0.0f;			//component with lower weight is run first inside the system object
		uint32_t ownId = 0;						//unique id for the component
		SystemObj *self = NULL;					//Parent system object that has access to all components

		//Higly recommended to use the Destroy function for safer deleting
		virtual ~CustomComponent();
		size_t GetComponentSize();						//Saving function, not useful for engine user
		virtual void *CollectSaveData(size_t &size);	//Saving function, not useful for engine user

		//Init is called right after the component is created
		//Should not rely on any other components or try to get components
		//In the Init function void* the tracked variables are returned first and then the memory chunks
		virtual void Init(void *data, size_t size) {};

		//Start is called once and guanranteed to happen before the first update
		//Will be called after each component has performed the Init function
		virtual void Start() {};

		//Updates every frame
		virtual void Update() {};

		//Update function that is called after updates and rendering
		virtual void LastUpdate() {};

		//Update function that is called at the same time as LastUpdate()
		//Is only called during engine mode is on, while other updates are not.
		virtual void EngineUpdate() {};

		//This is called right at the moment when the object is declared destroyed;
		virtual void OnDestroy() {};

		//Safe way to delete a component
		//The component is guaranteed to be around until the end of program cycle
		void Destroy();
};

#endif

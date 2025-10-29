
#ifndef SYS_SAVER_H
# define SYS_SAVER_H

# include "systemObj.h"
# include "unordered_map"
# include "snapshot.h"

# define FETCH_SIZE 1024 * 10
# define SNAPSHOT_AMOUNT 333

# define SNAPSHOT_PREVIOUS -1
# define SNAPSHOT_NEXT -2
# define SNAPSHOT_INDEX -3
# define SNAPSHOT_LATEST -4

struct SaveObjData
{
	uint32_t hash;
	uint32_t componentKey;
	uint32_t componentType;
	size_t compSize;
	void *data;
};

struct SaveObj
{
	uint32_t objHash;
	uint32_t saveable;
	std::vector<SaveObjData> components;
};

struct SnapObject
{
	size_t objSize;
	uint32_t objHash;
	uint64_t objKey;
	uint32_t saveable;
	std::vector<SaveObjData*> snapObj;
};

typedef std::vector<std::tuple<uint64_t, SystemObj*>> sysKeyObj;

// The Data in the snapShots is formatted like this:
// The data* only holds SystemObjects
// Each system object is formatted like this:
// [uint64_t  ], [uint32_t], [int     ], [uint32_t    ]
// [unique_key], [hash    ], [saveable], [size_of_data]
// Each systemObjects data is then formatted like this:
// [uint32_t      ], [uint32_t         ], [size]
// [component_type], [size_of_component], [data]

class SystemSaver
{
	private:
		//saving
		std::vector<std::tuple<uint64_t, uint32_t>> compDeletes = {};
		std::vector<uint64_t> objDeletes = {};

		void *dataFetcher = NULL;
		std::unordered_map<uint64_t, SaveObj> objectSaves;
		void AddNewObject(SystemObj *add);
		int FindFromVector(std::vector<SaveObjData> &components, uint32_t componentKey);
		void CheckExistingObject(SystemObj *check);
		bool HandleExistingObject(SaveObjData &existin, SystemObj *check, SaveObj &current);
		void AddNewComponentToObject(SystemObj *add, SaveObj &newAddition);
		void RemoveObjectFromSaverOwn(uint64_t key);

		//loading
		void *CreateImageComponent(void *data, size_t size);
		void *CreateStructureComponent(void *data, size_t size);
		void CreateComponentForSystemObject(SystemObj *obj, void *componentData, uint32_t componentType, size_t componentSize);
		SystemObj *GetSystemObjectFromData(void *data, sysKeyObj &store, uint16_t room);
	public:
		bool changeSpotted = false;
		SystemSaver();
		~SystemSaver();
		void ClearDeletingVectors();
		void SaveSystemObj(SystemObj *save);
		bool TakeSnapShot();
		std::vector<std::tuple<uint64_t, SystemObj*>> LoadSnapShot(SnapShot snapShot, uint16_t room);
		void RemoveObjectFromSaver(SystemObj *obj);
		void RemoveComponentFromSaver(uint64_t objKey, uint32_t compId);
};

#endif

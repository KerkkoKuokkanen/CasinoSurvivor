
#ifndef OBJECT_SELECTOR_H
# define OBJECT_SELECTOR_H

# include "systemObj.h"
# include <unordered_map>
# include "engineHierarchy.h"

class ObjectSelector
{
	private:
		bool init = false;
		EngineHierarchy *hieararchy = NULL;
		std::unordered_map<uint16_t, EngineHierarchy*> secondaryHieararchies;
		void SaveNodesData();
		void SetHierarchy();
		void InitSelector();
	public:
		~ObjectSelector();
		void SaveRoomChange(uint64_t objId, uint16_t room);
		void InitSecondaryHierarchy(void *second);
		std::tuple<uint64_t, bool, std::string> UpdateObjectSelector(std::unordered_map<uint64_t, SystemObj*> &objs, uint64_t self);
};

bool GetObjSelectorHover();

#endif

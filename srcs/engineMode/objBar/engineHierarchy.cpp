
#include "engineHierarchy.h"
#include "envHandler.h"
#include "objBar.h"

void EngineHierarchy::Init(void *data, size_t size)
{
	if (EngineModeOn() == false)
	{
		DestroyObject(self->GetSystemObjectKey());
		return ;
	}
	if (size <= 1)
		return ;
	size_t iter = 0;
	char *cast = (char*)data;
	while(iter < size)
	{
		NodeData add;
		memcpy(add.name, cast + iter, sizeof(char) * 48); iter += sizeof(char) * 48;
		memcpy(&add.is_folder, cast + iter, sizeof(bool)); iter += sizeof(bool);
		memcpy(&add.objKey, cast + iter, sizeof(uint64_t)); iter += sizeof(uint64_t);
		memcpy(&add.parent_id, cast + iter, sizeof(int)); iter += sizeof(int);
		currentData.push_back(add);
	}
	SaveHierarchy(currentData);
	ObjBar *bar = (ObjBar*)FindAny("ObjBar");
	if (bar == NULL)
		return ;
	bar->InitSecondaryHierarchy(this);
}

void EngineHierarchy::SaveHierarchy(std::vector<NodeData> &data)
{
	ClearSaveData();
	for (NodeData n : data)
	{
		AddToSave(n.name, sizeof(char) * 48);
		AddToSave(&n.is_folder, sizeof(bool));
		AddToSave(&n.objKey, sizeof(uint64_t));
		AddToSave(&n.parent_id, sizeof(int));
	}
}

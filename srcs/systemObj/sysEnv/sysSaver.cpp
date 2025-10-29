
#include "sysSaver.h"
#include "commonTools.h"
#include "snapShotCreator.h"
#include "envHandler.h"

void SystemSaver::AddNewComponentToObject(SystemObj *add, SaveObj &newAddition)
{
	changeSpotted = true;
	SaveObjData addition;
	size_t sizerTool = 0;
	addition.compSize = add->FetchComponentDataSize();
	if (addition.compSize == 0)
		return ;
	addition.componentKey = add->FetchComponentUniqueKey();
	addition.componentType = add->FetchComponentClassType();
	addition.data = malloc(addition.compSize);
	void *ret = add->FetchComponentSaveData(addition.data, addition.compSize, sizerTool);
	if (ret == NULL)
		printf("SystemSaver.cpp: AddNewComponent ret == NULL\n");
	if (addition.componentType >= 999) //The component type starting number for custom components
		memcpy(addition.data, ret, addition.compSize);
	else
		addition.data = ret;
	addition.hash = HashData32(ret, addition.compSize);
	newAddition.components.push_back(addition);
	newAddition.objHash ^= addition.hash;
}

void SystemSaver::AddNewObject(SystemObj *add)
{
	SaveObj newAddition = {0, add->saveable, {}};
	add->ResetComponentSaveFetching();
	while (add->ComponentFetchingAtEnd() == false)
	{
		AddNewComponentToObject(add, newAddition);
		add->IncrimentComponentFetching();
	}
	objectSaves[add->GetSystemObjectKey()] = newAddition;
}

int SystemSaver::FindFromVector(std::vector<SaveObjData> &components, uint32_t componentKey)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i].componentKey == componentKey)
			return (i);
	}
	return (-1);
}

bool SystemSaver::HandleExistingObject(SaveObjData &existing, SystemObj *check, SaveObj &current)
{
	void *fetcher = dataFetcher;
	size_t fetchSize = FETCH_SIZE;
	size_t sizerTool = 0;
	void *ret = check->FetchComponentSaveData(fetcher, fetchSize, sizerTool);
	if (ret == NULL)
		printf("sysSaver: dataFetcher was too small, custom components can not be the issue\n");
	uint32_t hash = HashData32(ret, sizerTool);
	if (existing.hash != hash)
	{
		AddNewComponentToObject(check, current);
		return (true);
	}
	return (false);
}

void SystemSaver::CheckExistingObject(SystemObj *check)
{
	uint64_t key = check->GetSystemObjectKey();
	auto objIndex = objectSaves.find(key);
	SaveObj &current = objIndex->second;
	current.saveable = check->saveable;
	check->ResetComponentSaveFetching();
	while (check->ComponentFetchingAtEnd() == false)
	{
		uint32_t componentKey = check->FetchComponentUniqueKey();
		int index = FindFromVector(current.components, componentKey);
		if (index == -1)
		{
			AddNewComponentToObject(check, current);
			check->IncrimentComponentFetching();
			continue ;
		}
		SaveObjData &handle = current.components[index];
		if (HandleExistingObject(handle, check, current))
		{
			free(current.components[index].data);
			current.components.erase(current.components.begin() + index);
		}
		check->IncrimentComponentFetching();
	}
}

void SystemSaver::ClearDeletingVectors()
{
	for (int i = 0; i < compDeletes.size(); i++)
	{
		uint64_t key = std::get<0>(compDeletes[i]);
		uint32_t compId = std::get<1>(compDeletes[i]);
		RemoveComponentFromSaver(key, compId);
	}
	for (int i = 0; i < objDeletes.size(); i++)
		RemoveObjectFromSaverOwn(objDeletes[i]);
	compDeletes.clear();
	objDeletes.clear();
}

void SystemSaver::SaveSystemObj(SystemObj *save)
{
	if (save == NULL)
		return ;
	uint64_t key = save->GetSystemObjectKey();
	if (objectSaves.find(key) == objectSaves.end())
		AddNewObject(save);
	else
		CheckExistingObject(save);
}

void SystemSaver::RemoveComponentFromSaver(uint64_t objKey, uint32_t compId)
{
	if (GetCreatingSnap())
	{
		compDeletes.push_back({objKey, compId});
		return ;
	}
	auto saveObj = objectSaves.find(objKey);
	if (saveObj == objectSaves.end())
		return ;
	SaveObj &delObj = saveObj->second;
	for (int i = 0; i < delObj.components.size(); i++)
	{
		if (delObj.components[i].componentKey == compId)
		{
			if (delObj.components[i].data != NULL)
				free(delObj.components[i].data);
			delObj.components.erase(delObj.components.begin() + i);
			return ;
		}
	}
}

void SystemSaver::RemoveObjectFromSaverOwn(uint64_t key)
{
	auto saveObj = objectSaves.find(key);
	if (saveObj == objectSaves.end())
		return ;
	SaveObj &delObj = saveObj->second;
	for (int i = 0; i < delObj.components.size(); i++)
	{
		if (delObj.components[i].data != NULL)
			free(delObj.components[i].data);
	}
	objectSaves.erase(key);
}

void SystemSaver::RemoveObjectFromSaver(SystemObj *obj)
{
	uint64_t key = obj->GetSystemObjectKey();
	if (GetCreatingSnap())
	{
		objDeletes.push_back(key);
		return ;
	}
	auto saveObj = objectSaves.find(key);
	if (saveObj == objectSaves.end())
		return ;
	SaveObj &delObj = saveObj->second;
	for (int i = 0; i < delObj.components.size(); i++)
	{
		if (delObj.components[i].data != NULL)
			free(delObj.components[i].data);
	}
	objectSaves.erase(key);
}

SystemSaver::SystemSaver()
{
	dataFetcher = malloc(FETCH_SIZE);
}

bool SystemSaver::TakeSnapShot()
{
	uint16_t room = GetCurrentRoom();
	bool ret = GiveSnapData(&objectSaves, room);
	return (ret);
}

SystemSaver::~SystemSaver()
{
	free(dataFetcher);
	for (auto &[key, save] : objectSaves)
	{
		SaveObj &obj = save;
		for (int i = 0; i < obj.components.size(); i++)
		{
			if (obj.components[i].data != NULL)
				free(obj.components[i].data);
		}
	}
}

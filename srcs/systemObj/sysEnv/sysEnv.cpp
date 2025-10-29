
#include "sysEnv.h"
#include "envHandler.h"
#include "saveInterface.h"
#include "keyboard.h"
#include "snapShotCreator.h"

bool engineMode = true;
bool overImgui = false;

bool EngineModeOn()
{
	return (engineMode);
}

void ChangeEngineMode(bool change)
{
	engineMode = change;
}

bool OverImgui()
{
	return (overImgui);
}

void ChangeOverImgui(bool change)
{
	overImgui = change;
}

SystemObj *SysEnv::FindObject(uint64_t key)
{
	auto obj = envSysObjs.find(key);
	if (obj == envSysObjs.end())
		return (NULL);
	return (obj->second);
}

void SysEnv::LastUpdateSysObjects()
{
	bool save = GetCreatingSnap();
	std::vector<SystemObj*> objs = {};
	objs.reserve(envSysObjs.size());
	for (const auto &[key, obj] : envSysObjs)
	{
		SystemObj *current = obj;
		objs.push_back(current);
	}
	std::sort(objs.begin(), objs.end(),
				[](const SystemObj *a, const SystemObj *b) {
					return a->weight < b->weight;
				});
	if (!save)
		envState->ClearDeletingVectors();
	if (deLoadedRooms.size() == 0)
	{
		for (SystemObj *obj : objs)
		{
			if (obj->active)
				obj->LastUpdateSystemObj();
			if (!save && (obj->GetSaveable() || engineMode))
				envState->SaveSystemObj(obj);
		}
	}
	else
	{
		for (SystemObj *obj : objs)
		{
			if (obj->active)
				obj->LastUpdateSystemObj();
			if (std::find(deLoadedRooms.begin(), deLoadedRooms.end(), obj->GetSaveableRoom()) != deLoadedRooms.end())
			{
				AddToDeleting(obj);
				continue ;
			}
			if (!save && (obj->GetSaveable() || engineMode))
				envState->SaveSystemObj(obj);
		}
		deLoadedRooms.clear();
	}
	for (int i = 0; i < compDeleting.size(); i++)
	{
		envState->changeSpotted = true;
		envState->RemoveComponentFromSaver(std::get<0>(compDeleting[i]), std::get<1>(compDeleting[i]));
	}
	for (int i = 0; i < deleting.size(); i++)
	{
		envState->changeSpotted = true;
		if (this->DeleteObject(deleting[i]->GetSystemObjectKey()) == false)
			delete deleting[i];
	}
	deleting.clear();
	if (envState->changeSpotted && !GetControlZ())
		SaveState();
	if (KeyHeld(SDL_SCANCODE_LCTRL) && KeyPressed(SDL_SCANCODE_P))
	{
		engineMode = true;
		RoomSwitch(GetCurrentRoom(), {0});
	}
}

void SysEnv::UpdateSysObjects()
{
	envState->changeSpotted = false;
	std::vector<SystemObj*> objs = {};
	objs.reserve(envSysObjs.size());
	for (const auto &[key, obj] : envSysObjs)
	{
		SystemObj *current = obj;
		objs.push_back(current);
	}
	std::sort(objs.begin(), objs.end(),
				[](const SystemObj *a, const SystemObj *b) {
					return a->weight < b->weight;
				});
	for (SystemObj *obj : objs)
	{
		if (obj->active)
			obj->UpdateSystemObj();
	}
}

bool SysEnv::DeleteObject(uint64_t key)
{
	auto eobj = envSysObjs.find(key);
	if (eobj == envSysObjs.end())
		return (false);
	envState->RemoveObjectFromSaver(eobj->second);
	eobj->second->controller = NULL;
	delete eobj->second;
	envSysObjs.erase(key);
	return (true);
}

void SysEnv::SnapLoading(sysKeyObj keyObj)
{
	if (keyObj.size() == 0)
		return ;
	for (int i = 0; i < keyObj.size(); i++)
	{
		uint64_t key = std::get<0>(keyObj[i]);
		SystemObj *obj = std::get<1>(keyObj[i]);
		RemoveObject(obj);
		obj->SetUniqueKeyManual(key);
		AddObject(obj);
	}
}

void SysEnv::LoadObjects(SnapShot &snap, uint16_t room)
{
	sysKeyObj ret = envState->LoadSnapShot(snap, room);
	SnapLoading(ret);
}

bool SysEnv::SaveState()
{
	bool ret = envState->TakeSnapShot();
	return (ret);
}

SysEnv::SysEnv()
{
	envState = new SystemSaver();
}

void SysEnv::Clear()
{
	for (const auto &[key, obj] : envSysObjs)
	{
		envState->RemoveObjectFromSaver(obj);
		obj->controller = NULL;
		delete obj;
	}
	envSysObjs.clear();
}

SysEnv::~SysEnv()
{
	delete envState;
	for (const auto &[key, obj] : envSysObjs)
	{
		SystemObj *current = obj;
		delete (current);
	}
}

void SysEnv::RemoveObject(SystemObj *obj)
{
	if (obj == NULL)
		return ;
	envState->RemoveObjectFromSaver(obj);
	uint64_t key = obj->GetSystemObjectKey();
	auto eobj = envSysObjs.find(key);
	if (eobj == envSysObjs.end())
		return ;
	envSysObjs.erase(key);
}

void SysEnv::AddObject(SystemObj *obj)
{
	if (obj == NULL)
		return ;
	uint64_t key = obj->GetSystemObjectKey();
	if (envSysObjs.find(key) != envSysObjs.end())
		return ;
	envState->changeSpotted = true;
	envSysObjs[key] = obj;
}

void *SysEnv::FindAny(std::string component)
{
	for (auto &[key, obj] : envSysObjs)
	{
		void *comp = obj->GetComponent(component);
		if (comp != NULL)
			return (comp);
	}
	return (NULL);
}

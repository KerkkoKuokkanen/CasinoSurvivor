
#include "snapshot.h"
#include "sysSaver.h"
#include "commonTools.h"
#include "saveSystem.h"
#include "saveInterface.h"
#include "snapShotCreator.h"
#include "roomLoading.h"
#include "envHandler.h"
#include "keyboard.h"
#include <mutex>

typedef std::tuple<uint16_t, std::unordered_map<uint64_t, SaveObj>*> dataHolder;

std::mutex boolMutex;		//For check boolean. SystemSaver uses this for thread safety
std::mutex saveBoolMutex;	//For save the state boolean
std::mutex holderMutex;		//This is for the data holder
std::mutex snapShotsMutex;	//For the snapshot ringbuffer and its index

bool creatingSnapShot = false;
bool saveState = false;
dataHolder holder = {0, NULL};

t_SnapSaves savedSnaps[SNAPSHOT_AMOUNT] = {0};
int currentIndex = 0;

void SetCreatingSnap(bool set)
{
	std::lock_guard<std::mutex> guard(boolMutex);
	creatingSnapShot = set;
}

bool GetCreatingSnap()
{
	std::lock_guard<std::mutex> guard(boolMutex);
	return (creatingSnapShot);
}

void SetSaveData(bool set)
{
	std::lock_guard<std::mutex> guard(saveBoolMutex);
	saveState = set;
}

bool GetSaveData()
{
	std::lock_guard<std::mutex> guard(saveBoolMutex);
	return (saveState);
}

bool GiveSnapData(std::unordered_map<uint64_t, SaveObj> *data, uint16_t callingRoom)
{
	if (GetCreatingSnap())
		return (false);
	SetCreatingSnap(true);
	std::lock_guard<std::mutex> guart(holderMutex);
	holder = {callingRoom, data};
	return (true);
}

t_SnapSaves CreateNewSnapShot()
{
	std::lock_guard<std::mutex> guart(holderMutex);
	std::vector<std::tuple<SnapShot, uint16_t>> snap = TakeSnapShot(std::get<1>(holder));
	return ((t_SnapSaves){1, snap, std::get<0>(holder)});
}

void AddNewSnapShotToBuffer(t_SnapSaves snap)
{
	std::lock_guard<std::mutex> guard(snapShotsMutex);
	currentIndex += 1;
	if (currentIndex >= SNAPSHOT_AMOUNT)
		currentIndex = 0;
	if (savedSnaps[currentIndex].has != 0)
	{
		for (auto &[snap, room] : savedSnaps[currentIndex].snap)
			free(snap.data);
	}
	savedSnaps[currentIndex] = snap;
}

t_SnapSaves CurrentState()
{
	std::lock_guard<std::mutex> guard(snapShotsMutex);
	return savedSnaps[currentIndex];
}

void ControlZ(bool *cz)
{
	std::lock_guard<std::mutex> guard(snapShotsMutex);
	if (KeyHeld(SDL_SCANCODE_LCTRL) && KeyPressed(SDL_SCANCODE_Z))
	{
		int index = currentIndex - 1;
		if (index < 0)
			index = SNAPSHOT_AMOUNT - 1;
		if (savedSnaps[index].has != 1)
			return ;
		*cz = true;
		savedSnaps[currentIndex].has = 0;
		for (auto &[snap, room] : savedSnaps[currentIndex].snap)
			free(snap.data);
		currentIndex = index;
		ClearSysEnv();
		for (auto &[snap, room] : savedSnaps[currentIndex].snap)
			LoadObjectsToEnvironment(snap, room);
	}
}

void UpdateSnapCreator()
{
	if (GetCreatingSnap())
	{
		t_SnapSaves snap = CreateNewSnapShot();
		AddNewSnapShotToBuffer(snap);
		SetCreatingSnap(false);
	}
	if (GetSaveData())
	{
		t_SnapSaves save = CurrentState();
		if (save.has == 1)
		{
			for (auto &[snap, room] : save.snap)
				SaveSnapShot(snap, GetRoomWithKey(room) + "/data");
		}
		SetSaveData(false);
	}
}

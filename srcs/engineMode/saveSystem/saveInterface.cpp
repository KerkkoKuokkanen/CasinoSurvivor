
#include "saveInterface.h"
#include "saveSystem.h"
#include <thread>
#include <mutex>

static uint32_t KeyGen()
{
	uint32_t keys = 0;
	keys += 1;
	if (keys > 4294967290)
		keys = 1;
	return (keys);
}

std::mutex functionMutex;
std::mutex saveInProcess;

bool saving = false;

std::vector<std::tuple<SnapShot, std::string>> saveables;
std::vector<std::tuple<uint32_t, std::string>> askedFiles = {};
std::unordered_map<uint32_t, void*> askedStates;

bool CollectAskedState(uint32_t key, void **collector)
{
	std::lock_guard<std::mutex> guard(functionMutex);
	auto it = askedStates.find(key);
	if (it == askedStates.end())
		return (false);
	*collector = it->second;
	askedStates.erase(key);
	return (true);
}

void SetAskedState(void *data, uint32_t key)
{
	std::lock_guard<std::mutex> guard(functionMutex);
	askedStates[key] = data;
}

uint32_t SetAskedData(std::string file)
{
	std::lock_guard<std::mutex> guard(functionMutex);
	uint32_t key = KeyGen();
	askedFiles.push_back({key, file});
	return (key);
}

std::tuple<uint32_t, std::string> CollectLastFile()
{
	std::tuple<uint32_t, std::string> ret = {0, ""};
	std::lock_guard<std::mutex> guard(functionMutex);
	if (askedFiles.size() == 0)
		return (ret);
	ret = askedFiles[0];
	askedFiles.erase(askedFiles.begin() + 0);
	return ret;
}

void SaveSnapShot(SnapShot snapShot, std::string file)
{
	std::lock_guard<std::mutex> guard(functionMutex);
	void *data = malloc(snapShot.size);
	memcpy(data, snapShot.data, snapShot.size);
	SnapShot use = {snapShot.hash, snapShot.size, data};
	saveables.push_back({use, file});
}

std::tuple<SnapShot, std::string> CollectFirstFromSnaps()
{
	std::lock_guard<std::mutex> guard(functionMutex);
	if (saveables.size() == 0)
		return ((std::tuple<SnapShot, std::string>){{0, 0, NULL}, ""});
	std::tuple<SnapShot, std::string> ret = saveables[0];
	saveables.erase(saveables.begin() + 0);
	return (ret);
}

void SetSaving(bool set)
{
	std::lock_guard<std::mutex> guard(saveInProcess);
	saving = set;
}

bool GetSaving()
{
	std::lock_guard<std::mutex> guard(saveInProcess);
	return (saving);
}

void SaveThread()
{
	std::tuple<SnapShot, std::string> ret = CollectFirstFromSnaps();
	SnapShot save = std::get<0>(ret);
	std::string file = std::get<1>(ret);
	if (save.data != NULL)
	{
		SetSaving(true);
		void *send = DataPrepping(save.data, save.size, save.hash);
		size_t offset = sizeof(uint64_t);
		uint8_t *castData = (uint8_t*)send;
		uint32_t size = *(uint32_t*)(castData + offset);
		SaveStateToFile(file.c_str(), send, (size_t)size + sizeof(uint32_t) + sizeof(uint64_t));
		free(send);
	}
	else
		SetSaving(false);
	std::tuple<uint32_t, std::string> loadFile = CollectLastFile();
	uint32_t fileKey = std::get<0>(loadFile);
	std::string fileName = std::get<1>(loadFile);
	if (fileName != "")
	{
		void *date = LoadStateFromFile(fileName.c_str());
		if (CorruptionCheck(date) == true)
			SetAskedState(date, fileKey);
		else
		{
			if (date != NULL)
				free(date);
			std::string fileName2 = fileName + "c";
			void *date2 = LoadStateFromFile(fileName2.c_str());
			if (CorruptionCheck(date2) == true)
				SetAskedState(date2, fileKey);
			else
			{
				if (date2 != NULL)
					free(date2);
				SetAskedState(NULL, fileKey);
			}
		}
	}
}

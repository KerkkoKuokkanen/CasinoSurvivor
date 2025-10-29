
#include "commonTools.h"
#include "snapshot.h"
#include "sysSaver.h"

void SetSnapObjects(std::vector<SnapObject> &setted, SaveObj &current, size_t &totalSize, uint64_t key)
{
	SnapObject addition = {0, current.objHash, key, current.saveable, {}};
	for (int i = 0; i < current.components.size(); i++)
	{
		addition.snapObj.push_back(&current.components[i]);
		addition.objSize += current.components[i].compSize;
		addition.objSize += sizeof(uint32_t);
		addition.objSize += sizeof(uint32_t);
		totalSize += current.components[i].compSize;
		totalSize += sizeof(uint32_t);
		totalSize += sizeof(uint32_t);
	}
	setted.push_back(addition);
}

void SetToSnapData(uint8_t *snap, std::vector<SnapObject> &saveObjs)
{
	size_t offset = 0;
	for (int i = 0; i < saveObjs.size(); i++)
	{
		uint32_t saveObjSize = (uint32_t)saveObjs[i].objSize;
		memcpy(snap + offset, &saveObjs[i].objKey, sizeof(uint64_t)); offset += sizeof(uint64_t);
		memcpy(snap + offset, &saveObjs[i].objHash, sizeof(uint32_t)); offset += sizeof(uint32_t);
		memcpy(snap + offset, &saveObjs[i].saveable, sizeof(int)); offset += sizeof(int);
		memcpy(snap + offset, &saveObjSize, sizeof(uint32_t)); offset += sizeof(uint32_t);
		for (int j = 0; j < saveObjs[i].snapObj.size(); j++)
		{
			SaveObjData *sod = saveObjs[i].snapObj[j];
			uint32_t compSize = (uint32_t)sod->compSize;
			memcpy(snap + offset, &sod->componentType, sizeof(uint32_t)); offset += sizeof(uint32_t);
			memcpy(snap + offset, &compSize, sizeof(uint32_t)); offset += sizeof(uint32_t);
			memcpy(snap + offset, sod->data, (size_t)sod->compSize); offset += (size_t)sod->compSize;
		}
	}
}

typedef std::unordered_map<uint16_t, std::vector<std::tuple<uint64_t, SaveObj>>> sortedObjects;

sortedObjects SortObjects(std::unordered_map<uint64_t, SaveObj> *objectSaves)
{
	sortedObjects sorted;
	for (auto &[key, obj] : *objectSaves)
	{
		uint16_t room = (uint16_t)(obj.saveable >> 16);
		sorted[room].push_back({key, obj});
	}
	return sorted;
}

SnapShot MakeSnapShot(std::vector<std::tuple<uint64_t, SaveObj>> &objectSaves)
{
	size_t totalSize = 0;
	std::vector<SnapObject> saveObjs;
	for (auto &[key, obj] : objectSaves)
		SetSnapObjects(saveObjs, obj, totalSize, key);
	size_t newSize = totalSize + (sizeof(uint32_t) * saveObjs.size() * 2)
								+ (sizeof(int) * saveObjs.size())
								+ (sizeof(uint64_t) * saveObjs.size());
	void *snap = malloc(newSize);
	SetToSnapData((uint8_t*)snap, saveObjs);
	uint64_t hash = HashData64(snap, newSize);
	return ((SnapShot){hash, (uint32_t)newSize, snap});
}

std::vector<std::tuple<SnapShot, uint16_t>> TakeSnapShot(std::unordered_map<uint64_t, SaveObj> *objectSaves)
{
	std::vector<std::tuple<SnapShot, uint16_t>> ret = {};
	sortedObjects sorted = SortObjects(objectSaves);
	for (auto &[key, vec] : sorted)
	{
		SnapShot snap = MakeSnapShot(vec);
		ret.push_back({snap, key});
	}
	return (ret);
}

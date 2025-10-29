
#include <random>
#include "componentRegistry.h"
#include "sysEnv.h"
#include "envHandler.h"
#include "image.h"

static bool SortComponents(const t_sysComponent& a, const t_sysComponent& b)
{
	return a.classType > b.classType;
}

static uint64_t GenerateRandomNumber()
{
	static std::mt19937_64 gen(std::random_device{}());
	static std::uniform_int_distribution<uint64_t> distrib(0, (1ULL << 42) - 1);
	return distrib(gen);
}

static uint32_t GetUniqueKeyForSysComponent()
{
	static uint32_t key = rand() % 4294967293;
	key += 1;
	if (key >= 4294967293)
		key = 0;
	return (key);
}

static uint64_t GetUniqueKeyForSysObj()
{
	static uint64_t key = rand() % 4194304;
	uint64_t randomPart = GenerateRandomNumber();
	key += 1;
	if (key >= 4194304)
		key = 0;
	uint64_t ret = (key << 42) | randomPart;
	return (ret);
}

void *SystemObj::AddImageComponent()
{
	Image *img = new Image("everyColor", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 0);
	uint32_t id = GetUniqueKeyForSysComponent();
	t_sysComponent add = {id, n_ComponentTypes::IMAGE_CLASS, false, "image", img};
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
	GiveComponentId(img, n_ComponentTypes::IMAGE_CLASS, id);
	return (img);
}

void SystemObj::AddObjectController(void *controller)
{
	if (controller == NULL)
		return ;
	SystemObj::controller = controller;
	SysEnv *env = (SysEnv*)controller;
	env->AddObject(this);
}

SystemObj::SystemObj()
{
	uniqueSystemObjKey = GetUniqueKeyForSysObj();
	controller = GetCurrentEnvironment();
	uint16_t room = GetCurrentRoom();
	saveable |= (uint32_t)room << 16;
	if (controller == NULL)
		return ;
	SysEnv *env = (SysEnv*)controller;
	env->AddObject(this);
}

void *SystemObj::AddComponent(const std::string component, void *initData, size_t initDataSize)
{
	t_sysComponent add;
	add.started = false;
	add.uniqueKey = GetUniqueKeyForSysComponent();
	add.obj = (void*)CreateComponent(component);
	CustomComponent *comp = (CustomComponent*)add.obj;
	comp->self = this;
	if (initDataSize == sizeof(int8_t))
	{
		int8_t checker = *(int8_t*)initData;
		if (checker == -128)
			comp->Init(NULL, 0);
		else
			comp->Init(initData, initDataSize);
	}
	else
		comp->Init(initData, initDataSize);
	add.type = component;
	add.classType = GetComponentKeyWithName(component);
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
	GiveComponentId(add.obj, add.classType, add.uniqueKey);
	return (add.obj);
}

void *SystemObj::AddComponent(const std::string component)
{
	if (component == "image")
		return (AddImageComponent());
	t_sysComponent add;
	add.started = false;
	add.uniqueKey = GetUniqueKeyForSysComponent();
	add.obj = (void*)CreateComponent(component);
	if (add.obj == NULL)
		return (NULL);
	CustomComponent *comp = (CustomComponent*)add.obj;
	comp->self = this;
	comp->Init(NULL, 0);
	add.type = component;
	add.classType = GetComponentKeyWithName(component);
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
	GiveComponentId(add.obj, add.classType, add.uniqueKey);
	return (add.obj);
}

void *SystemObj::AddComponent(void *component, const std::string name)
{
	uint32_t classType = GetComponentKeyWithName(name);
	if (name == IMAGE_COMPONENT)
		classType = n_ComponentTypes::IMAGE_CLASS;
	else if (name == STRUCTURE_COMPONENT)
		classType = n_ComponentTypes::STRUCTURE_CLASS;
	uint32_t id = GetUniqueKeyForSysComponent();
	t_sysComponent add = {id, classType, false, name, component};
	CustomComponent *comp = (CustomComponent*)component;
	comp->Init(NULL, 0);
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
	GiveComponentId(component, classType, id);
	return (component);
}

void *SystemObj::AddComponent(void *component, uint32_t classType)
{
	uint32_t id = GetUniqueKeyForSysComponent();
	const std::string name = GetComponentNameWithKey(classType);
	t_sysComponent add = {id, classType, false, name, component};
	components.push_back(add);
	std::sort(components.begin(), components.end(), SortComponents);
	GiveComponentId(component, classType, id);
	return (component);
}

void *SystemObj::GetComponent(const std::string &component)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (component == components[i].type)
			return (components[i].obj);
	}
	return (NULL);
}

std::vector<void*> SystemObj::GetComponents(const std::string &component)
{
	std::vector<void*> ret = {};
	for (int i = 0; i < components.size(); i++)
	{
		if (component == components[i].type)
			ret.push_back(components[i].obj);
	}
	return (ret);
}

bool SystemObj::ComponentFetchingAtEnd()
{
	if (componentSaveFetchIndex >= components.size())
		return (true);
	return (false);
}

uint32_t SystemObj::FetchComponentClassType()
{
	if (componentSaveFetchIndex >= components.size())
		return (n_ComponentTypes::NO_CLASS);
	return (components[componentSaveFetchIndex].classType);
}

uint32_t SystemObj::FetchComponentUniqueKey()
{
	if (componentSaveFetchIndex >= components.size())
		return (0);
	return (components[componentSaveFetchIndex].uniqueKey);
}

uint16_t SystemObj::GetSaveableRoom()
{
	uint16_t room = (uint16_t)(saveable >> 16);
	return (room);
}

void SystemObj::SetSaveable(uint8_t save, uint16_t room)
{
	if (room == 0)
		room = GetCurrentRoom();
	saveable = 0;
	saveable |= (uint32_t)room << 16;
	saveable |= (uint32_t)save;
}

bool SystemObj::GetSaveable()
{
	uint8_t check = (uint8_t)saveable;
	if (check != 0)
		return (true);
	return (false);
}

void SystemObj::RemoveComponent(uint32_t id)
{
	if (deleting)
		return ;
	ComponentRemover(uniqueSystemObjKey, id);
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i].uniqueKey == id)
		{
			components.erase(components.begin() + i);
			return ;
		}
	}
}

void SystemObj::Destroy()
{
	DestroyObject(this);
	deleting = true;
}


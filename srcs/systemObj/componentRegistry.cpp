
#include "componentRegistry.h"
#include "customComponent.h"
#include <unordered_map>
#include "commonTools.h"

#define COMP_SIGN_START 999

static std::unordered_map<std::string, ComponentFactory> &GetComponentRegistry()
{
	static std::unordered_map<std::string, ComponentFactory> componentFactories;
	return (componentFactories);
}

static std::unordered_map<unsigned int, ComponentFactory> &GetComponentSignRegistry()
{
	static std::unordered_map<unsigned int, ComponentFactory> componentFactories;
	return (componentFactories);
}

static std::unordered_map<std::string, unsigned int> &GetComponentNameRegistry()
{
	static std::unordered_map<std::string, unsigned int> componentNames;
	return (componentNames);
}

static std::unordered_map<unsigned int, std::string> &GetComponentIntRegistry()
{
	static std::unordered_map<unsigned int, std::string> componentNames;
	return (componentNames);
}

static unsigned int GetUniqueComponentKey(std::string name)
{
	uint32_t key = HashData32(name.data(), name.length());
	return (key);
}

void RegisterComponent(const std::string &type, ComponentFactory factory)
{
	auto &registry = GetComponentRegistry();
	auto &signRegistry = GetComponentSignRegistry();
	auto &intRegistry = GetComponentIntRegistry();
	auto &nameRegistry = GetComponentNameRegistry();
	if (registry.find(type) == registry.end())
	{
		unsigned int key = GetUniqueComponentKey(type);
		registry[type] = factory;
		signRegistry[key] = factory;
		intRegistry[key] = type;
		nameRegistry[type] = key;
	}
}

std::string GetComponentNameWithKey(unsigned int key)
{
	if (key == n_ComponentTypes::IMAGE_CLASS)
		return (IMAGE_COMPONENT);
	if (key == n_ComponentTypes::STRUCTURE_CLASS)
		return (STRUCTURE_COMPONENT);
	auto &intRegistry = GetComponentIntRegistry();
	auto it = intRegistry.find(key);
	if (it != intRegistry.end())
		return (it->second);
	return ("");
}

unsigned int GetComponentKeyWithName(const std::string &name)
{
	if (name == IMAGE_COMPONENT)
		return (n_ComponentTypes::IMAGE_CLASS);
	if (name == STRUCTURE_COMPONENT)
		return (n_ComponentTypes::STRUCTURE_CLASS);
	auto &nameRegistry = GetComponentNameRegistry();
	auto it = nameRegistry.find(name);
	if (it != nameRegistry.end())
		return (it->second);
	return (0);
}

CustomComponent *CreateComponent(const std::string &type)
{
	auto &registry = GetComponentRegistry();
	auto it = registry.find(type);
	if (it != registry.end())
		return (it->second());
	return (NULL);
}

CustomComponent* CreateComponent(unsigned int id)
{
	auto& registry = GetComponentSignRegistry();
	auto it = registry.find(id);
	if (it != registry.end())
		return (it->second());
	return (NULL);
}

std::vector<std::string> GetAllComponentNames()
{
	std::unordered_map<unsigned int, std::string> componentNames = GetComponentIntRegistry();
	std::vector<std::string> names;
	for (auto it : componentNames)
		names.push_back(it.second);
	return (names);
}

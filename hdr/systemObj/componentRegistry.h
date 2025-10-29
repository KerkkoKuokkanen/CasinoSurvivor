
#ifndef COMPONENT_REGISTRY_H
# define COMPONENT_REGISTRY_H

# include <iostream>
# include "customComponent.h"

typedef std::function<CustomComponent*()> ComponentFactory;

void RegisterComponent(const std::string &type, ComponentFactory factory);
CustomComponent *CreateComponent(unsigned int id);
CustomComponent *CreateComponent(const std::string &type);

std::string GetComponentNameWithKey(unsigned int key);
unsigned int GetComponentKeyWithName(const std::string &name);

std::vector<std::string> GetAllComponentNames();

#define REGISTER_COMPONENT(TYPE) \
	namespace { \
		const bool registered_##TYPE = [] { \
			RegisterComponent(#TYPE, []() -> CustomComponent* { return new TYPE(); }); \
			return true; \
		}(); \
	}

#endif

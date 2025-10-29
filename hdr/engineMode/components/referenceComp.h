
#ifndef REFERENCE_COMPONENT_H
# define REFERENCE_COMPONENT_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "unordered_map"

class Reference : public CustomComponent
{
	private:
		char inputName[32] = {0};
		char inputKey[32] = {0};
		bool added = false;
	public:
		Reference();
		std::unordered_map<std::string, uint64_t> references;
		void Init(void *data, size_t size) override;
		void EngineUpdate() override;
};

REGISTER_COMPONENT(Reference);

#endif

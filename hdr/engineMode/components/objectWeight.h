
#ifndef OBJECT_WEIGHT_H
# define OBJECT_WEIGHT_H

# include "customComponent.h"
# include "componentRegistry.h"

class ObjectWeight : public CustomComponent
{
	private:
		float weight = 0.0f;
	public:
		void Init(void *data, size_t size) override;
		void Start() override;
};

REGISTER_COMPONENT(ObjectWeight);

#endif

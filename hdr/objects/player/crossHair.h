
#ifndef CROSS_HAIR_H
# define CROSS_HAIR_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"

class CrossHair : public CustomComponent
{
	private:
		Image *img = NULL;
	public:
		~CrossHair();
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(CrossHair);

#endif

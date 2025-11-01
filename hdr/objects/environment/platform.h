
#ifndef PLATFORM_H
# define PLATFORM_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"

class Platform : public CustomComponent
{
	private:
		Image *img = NULL;
	public:
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(Platform);

#endif

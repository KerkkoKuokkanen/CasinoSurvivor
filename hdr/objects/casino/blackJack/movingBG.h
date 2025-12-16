
#ifndef MOVING_BG_H
# define MOVING_BG_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"

class MovingBG : public CustomComponent
{
	private:
		Image *img = NULL;
	public:
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(MovingBG);

#endif


#ifndef SCREEN_TRANSITION_H
# define SCREEN_TRANSITION_H

#include "component.h"

class ScreenTransition : public CustomComponent
{
	private:
		Image *cover = NULL;
		float time = 1.0f;
	public:
		bool done = false;
		ScreenTransition();
		~ScreenTransition();
		void Update() override;
		void EngineUpdate() override;
};

REGISTER_COMPONENT(ScreenTransition);

#endif

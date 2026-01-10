
#ifndef SCREEN_TRANSITION_H
# define SCREEN_TRANSITION_H

#include "component.h"

class ScreenTransition : public CustomComponent
{
	private:
		Image *cover = NULL;
		int transitionType = 0;
	public:
		float time = 1.0f;
		bool removeSelf = true;
		bool done = false;
		ScreenTransition();
		~ScreenTransition();
		void Init(void *data, size_t size) override;
		void Update() override;
		void EngineUpdate() override;
};

REGISTER_COMPONENT(ScreenTransition);

#endif

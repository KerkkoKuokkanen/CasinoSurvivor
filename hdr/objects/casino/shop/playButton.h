
#ifndef PLAY_BUTTON_H
# define PLAY_BUTTON_H

# include "component.h"
# include "screenTransition.h"
# include "button.h"

class PlayButton : public CustomComponent
{
	private:
		Image *playTextFront = NULL;
		Image *playTextBack = NULL;
		Image *front = NULL;
		Image *back = NULL;
		Button *button = NULL;
		ScreenTransition *screenTrans = NULL;
		float fadeTimer = 0.0f;
		float fadeIndex = 20.0f;
		bool play = false;
	public:
		void SetPlay();
		PlayButton();
		~PlayButton();
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(PlayButton);

#endif

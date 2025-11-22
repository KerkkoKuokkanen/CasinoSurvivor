
#ifndef BUTTON_H
# define BUTTON_H

# include "image.h"

struct ButtonState
{
	Image *button;
	t_Box color;
};

class Button
{
	private:
		ButtonState states[4] = {0};
		void (*callFunc)() = NULL;
		void GetClickState();
	public:
		bool notUsable = false;
		bool active = true;
		int clickState = 0;
		void SetButtonFunction(void (*fun)());
		void SetButtonState(int state, Image *img, t_Box color);
		void Update();
};

#endif

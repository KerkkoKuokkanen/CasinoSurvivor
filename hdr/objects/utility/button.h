
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
		std::function<void()> f = nullptr;
		void GetClickState();
	public:
		bool notUsable = false;
		bool active = true;
		int clickState = 0;
		void SetButtonFunction(std::function<void()> func);
		//click states 0 - 3
		// 0: button not hovered
		// 1: button hovered
		// 2: holding click
		// 3: not usable
		void SetButtonState(int state, Image *img, t_Box color);
		void Update();
};

#endif

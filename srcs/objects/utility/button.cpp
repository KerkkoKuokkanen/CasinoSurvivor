
#include "button.h"
#include "mouseOver.h"
#include "mouse.h"

void Button::SetButtonFunction(void (*fun)())
{
	callFunc = fun;
}

void Button::SetButtonState(int state, Image *img, t_Box color)
{
	if (state < 0 || state > 3)
		return ;
	states[state].button = img;
	states[state].color = color;
}

void Button::GetClickState()
{
	bool ret = MouseOverImage(states[0].button);
	if (ret == false)
	{
		clickState = 0;
		return ;
	}
	clickState = 1;
	if (MouseKeyHeld(n_MouseKeys::MOUSE_LEFT))
		clickState = 2;
	if (MouseKeyReleased(n_MouseKeys::MOUSE_LEFT))
		clickState = 3;
}

void Button::Update()
{
	GetClickState();
	if (notUsable)
	{
		t_Box color = states[3].color;
		states[0].button->drawActive = false;
		states[1].button->drawActive = false;
		states[2].button->drawActive = false;
		states[3].button->drawActive = true;
		states[3].button->SetColor(color.x, color.y, color.w, color.h);
		return ;
	}
	if (active == false)
	{
		t_Box color = states[0].color;
		states[1].button->drawActive = false;
		states[2].button->drawActive = false;
		states[3].button->drawActive = false;
		states[0].button->drawActive = true;
		states[0].button->SetColor(color.x, color.y, color.w, color.h);
		return ;
	}
	if (clickState == 1)
	{
		t_Box color = states[1].color;
		states[0].button->drawActive = false;
		states[2].button->drawActive = false;
		states[3].button->drawActive = false;
		states[1].button->drawActive = true;
		states[1].button->SetColor(color.x, color.y, color.w, color.h);
		return ;
	}
	if (clickState >= 2)
	{
		t_Box color = states[2].color;
		states[0].button->drawActive = false;
		states[1].button->drawActive = false;
		states[3].button->drawActive = false;
		states[2].button->drawActive = true;
		states[2].button->SetColor(color.x, color.y, color.w, color.h);
		if (clickState == 3 && callFunc != NULL)
			callFunc();
		return ;
	}
	t_Box color = states[0].color;
	states[1].button->drawActive = false;
	states[2].button->drawActive = false;
	states[3].button->drawActive = false;
	states[0].button->drawActive = true;
	states[0].button->SetColor(color.x, color.y, color.w, color.h);
}


#include "mouse.h"
#include "screen.h"
#include "imageTransforms.h"

int mouseX = 0;
int mouseY = 0;
int wheel = 0;
Uint32 mouseState = 0;

int mouseLeft = 0;
int mouseRight = 0;
int mouseMiddle = 0;
int mouse4 = 0;
int mouse5 = 0;

t_Point GetMouseXY()
{
	float unitX = 2.0f / (float)__currentScreenWidth;
	float unitY = 2.0f / (float)__currentScreenHeight;

	t_Point ret;
	ret.x = unitX * (float)mouseX - 1.0f;
	ret.y = (unitY * (float)mouseY - 1.0f) * (-1.0f);
	ret.x *= 10.0f;
	ret.y *= 10.0f;
	return (ret);
}

t_Point GetMouseXYZoom()
{
	float unitX = (2.0f / (float)__currentScreenWidth);
	float unitY = (2.0f / (float)__currentScreenHeight);

	t_Point ret;
	ret.x = unitX * (float)mouseX - 1.0f;
	ret.y = (unitY * (float)mouseY - 1.0f) * (-1.0f);
	ret.x *= (__ScreenSpaceUsedWidth * 0.1f);
	ret.y *= (__ScreenSpaceUsedHeight * 0.1f);
	ret.x *= 10.0f;
	ret.y *= 10.0f;
	return (ret);
}

t_Point GetMouseXYCamera()
{
	float unitX = 2.0f / (float)__currentScreenWidth;
	float unitY = 2.0f / (float)__currentScreenHeight;

	t_Point ret;
	ret.x = unitX * (float)mouseX - 1.0f;
	ret.y = (unitY * (float)mouseY - 1.0f) * (-1.0f);
	ret.x *= (__ScreenSpaceUsedWidth * 0.1f);
	ret.y *= (__ScreenSpaceUsedHeight * 0.1f);
	ret.x *= 10.0f;
	ret.y *= 10.0f;
	ret.x += __CameraX;
	ret.y += __CameraY;
	return (ret);
}

std::tuple<int, int> GetRawMouseXY()
{
	std::tuple<int, int> ret = {mouseX, mouseY};
	return (ret);
}

bool WheelIn()
{
	if (wheel > 0)
		return (true);
	return (false);
}

bool WheelOut()
{
	if (wheel < 0)
		return (true);
	return (false);
}

int MouseWheel()
{
	return (wheel);
}

static int GetChecked(int mouseKey)
{
	int checked = 0;
	switch (mouseKey)
	{
		case n_MouseKeys::MOUSE_LEFT:
			checked = mouseLeft;
			break ;
		case n_MouseKeys::MOUSE_RIGHT:
			checked = mouseRight;
			break ;
		case n_MouseKeys::MOUSE_MIDDLE:
			checked = mouseMiddle;
			break ;
		case n_MouseKeys::MOUSE_4:
			checked = mouse4;
			break ;
		case n_MouseKeys::MOUSE_5:
			checked = mouse5;
			break ;
	}
	return (checked);
}

bool MouseKeyPressed(int mouseKey)
{
	int checked = GetChecked(mouseKey);
	if (checked == 1)
		return (true);
	return (false);
}

bool MouseKeyHeld(int mouseKey)
{
	int checked = GetChecked(mouseKey);
	if (checked > 0)
		return (true);
	return (false);
}

bool MouseKeyReleased(int mouseKey)
{
	int checked = GetChecked(mouseKey);
	if (checked == (-1))
		return (true);
	return (false);
}

void UpdateMouse()
{
	mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	wheel = GetScrollWheelValue();

	if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
		mouseLeft++;
	else if (mouseLeft != -1 && mouseLeft != 0)
		mouseLeft = -1;
	else
		mouseLeft = 0;

	if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
		mouseRight++;
	else if (mouseRight != 0 && mouseRight != (-1))
		mouseRight = -1;
	else
		mouseRight = 0;

	if (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		mouseMiddle++;
	else if (mouseMiddle != -1 && mouseMiddle != 0)
		mouseMiddle = -1;
	else
		mouseMiddle = 0;

	if (mouseState & SDL_BUTTON(SDL_BUTTON_X1))
		mouse4++;
	else if (mouse4 != -1 && mouse4 != 0)
		mouse4 = -1;
	else
		mouse4 = 0;

	if (mouseState & SDL_BUTTON(SDL_BUTTON_X2))
		mouse5++;
	else if (mouse5 != -1 && mouse5 != 0)
		mouse5 = -1;
	else
		mouse5 = 0;
}


#include "crossHair.h"
#include "mouse.h"
#include <SDL.h>

CrossHair::~CrossHair()
{
	SDL_ShowCursor(SDL_ENABLE);
	if (img != NULL)
		delete img;
}

void CrossHair::Start()
{
	SDL_ShowCursor(SDL_DISABLE);
	img = new Image("crossHair1", {0.0f, 0.0f, 0.33f, 0.33f}, 0.0f, 16);
	img->SetColor(1.0f, 1.0f, 1.0f, 0.95f);
}

void CrossHair::Update()
{
	t_Point pos = GetMouseXY();
	img->position = pos;
}

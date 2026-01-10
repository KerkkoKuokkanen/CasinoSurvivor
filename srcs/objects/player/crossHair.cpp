
#include "crossHair.h"
#include "mouse.h"
#include <SDL.h>

#define CH_SIZE 0.12f
#define CH_SIZE2 0.1f

CrossHair::~CrossHair()
{
	SDL_ShowCursor(SDL_ENABLE);
	if (img[0] != NULL)
		delete img[0];
	if (img[1] != NULL)
		delete img[1];
	if (img[2] != NULL)
		delete img[2];
	if (img[3] != NULL)
		delete img[3];
	if (img[4] != NULL)
		delete img[4];
}

void CrossHair::Start()
{
	SDL_ShowCursor(SDL_DISABLE);
	img[0] = new Image("ch2", {0.0f, 0.0f, CH_SIZE, CH_SIZE}, 0.0f, 16);
	img[0]->SetColor(0.9f, 0.9f, 0.9f, 1.0f);
	img[0]->drawDepth = 1.0f;
	img[1] = new Image("ch2", {0.0f, 0.0f, CH_SIZE, CH_SIZE}, 0.0f, 16);
	img[1]->SetColor(0.9f, 0.9f, 0.9f, 1.0f);
	img[2] = new Image("ch2", {0.0f, 0.0f, CH_SIZE, CH_SIZE}, 0.0f, 16);
	img[2]->SetColor(0.9f, 0.9f, 0.9f, 1.0f);
	img[3] = new Image("ch2", {0.0f, 0.0f, CH_SIZE, CH_SIZE}, 0.0f, 16);
	img[3]->SetColor(0.9f, 0.9f, 0.9f, 1.0f);
	img[4] = new Image("ch2", {0.0f, 0.0f, CH_SIZE, CH_SIZE}, 0.0f, 16);
	img[4]->SetColor(0.9f, 0.9f, 0.9f, 1.0f);
	img[0]->angle = PI * 0.25f;
	img[1]->angle = PI * 0.25f;
	img[2]->angle = PI * 0.25f;
	img[3]->angle = PI * 0.25f;
	img[4]->angle = PI * 0.25f;
}

void CrossHair::Update()
{
	t_Point pos = GetMouseXY();
	float diff = CH_SIZE2 + recoil * 0.25f;
	img[0]->position = pos;
	img[1]->position = {pos.x - diff, pos.y};
	img[2]->position = {pos.x + diff, pos.y};
	img[3]->position = {pos.x, pos.y - diff};
	img[4]->position = {pos.x, pos.y + diff};
}


#include "chipMove.h"

ChipMove::ChipMove()
{
	img = new Image("everyColor", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 5);
}

void ChipMove::SetUp(int chip, int destination)
{
	switch (chip)
	{
		case 0:
			img->position = {-2.0f, -5.0f};
			img->SetTexture("chip1");
			break ;
		case 1:
			img->position = {-1.2f, -5.0f};
			img->SetTexture("chip5");
			break ;
		case 2:
			img->position = {-0.4f, -5.0f};
			img->SetTexture("chip10");
			break ;
		case 3:
			img->position = {0.4f, -5.0f};
			img->SetTexture("chip50");
			break ;
		case 4:
			img->position = {1.2f, -5.0f};
			img->SetTexture("chip100");
			break ;
		default:
			img->position = {2.0f, -5.0f};
			img->SetTexture("chip500");
			break ;
	}
	if (destination == 0)
		dest = {0.0f, -3.17f};
	else if (destination == 1)
		dest = {1.76f, -3.07f};
	else
		dest = {-1.75f, -3.06f};
	dir = {dest.x - img->position.x, dest.y - img->position.y};
	dir = VectorNormalize(dir);
}

ChipMove::~ChipMove()
{
	delete img;
}

void ChipMove::Update()
{
	t_Point pos = img->position;
	pos.x += dir.x * 8.0f * DeltaTime();
	pos.y += dir.y * 8.0f * DeltaTime();
	img->position = pos;
	if (pos.y >= (dest.y - 0.01f))
		RemoveSelf();
}


#include "player.h"
#include "deltaTime.h"

#define X_ADD 5.5f

void PlayerMovement::ShoesWhilePressedF(float x)
{
	if (direction.x < 0.0f)
		return ;
	float y = -(x * x) + x;
	float xAdd = (X_ADD / 6.5f) * fabs(direction.x);
	t_Point pos = torso->position;
	if (!leftSliding)
	{
		leftS->position.x = pos.x + x * 0.5f - 0.1f;
		leftS->position.y = pos.y + y;
		if (!firstCycle)
			rightS->position.x = (pos.x + 0.5f) - x * 0.5f - 0.2f;
		else
			rightS->position = torso->position;
		if (x + xAdd * DeltaTime() >= 1.0f)
			leftSliding = true;
	}
	else
	{
		rightS->position.x = pos.x + x * 0.5f - 0.2f;
		rightS->position.y = pos.y + y;
		if (!firstCycle)
			leftS->position.x = (pos.x + 0.5f) - x * 0.5f - 0.1f;
		else
			leftS->position = torso->position;
		if (x + xAdd * DeltaTime() >= 1.0f)
			leftSliding = false;
	}
}

void PlayerMovement::ShoesWhilePressedB(float x)
{
	if (direction.x >= 0.0f)
		return ;
	float y = -(x * x) + x;
	float xAdd = (X_ADD / 6.5f) * fabs(direction.x);
	t_Point pos = torso->position;
	if (!leftSliding)
	{
		leftS->position.x = pos.x - x * 0.5f + 0.1f;
		leftS->position.y = pos.y + y;
		if (!firstCycle)
			rightS->position.x = (pos.x - 0.5f) + x * 0.5f + 0.2f;
		else
			rightS->position = torso->position;
		if (x + xAdd * DeltaTime() >= 1.0f)
			leftSliding = true;
	}
	else
	{
		rightS->position.x = pos.x - x * 0.5f + 0.2f;
		rightS->position.y = pos.y + y;
		if (!firstCycle)
			leftS->position.x = (pos.x - 0.5f) + x * 0.5f + 0.1f;
		else
			leftS->position = torso->position;
		if (x + xAdd * DeltaTime() >= 1.0f)
			leftSliding = false;
	}
}

bool PlayerMovement::PositionShoes()
{
	static float calc = 0.0;
	bool inPos = true;
	t_Point pos1 = leftS->position;
	t_Point pos2 = rightS->position;
	t_Point home = torso->position;
	t_Point dir1 = {home.x - pos1.x, home.y - pos1.y};
	t_Point dir2 = {home.x - pos2.x, home.y - pos2.y};
	if (fabs(dir1.x) + fabs(dir1.y) < 0.1f)
		leftS->position = home;
	else
	{
		inPos = false;
		leftS->position = {pos1.x + dir1.x * 19.0f * DeltaTime(), pos1.y + dir1.y * 19.0f * DeltaTime()};
	}
	if (fabs(dir2.x) + fabs(dir2.y) < 0.1f)
		rightS->position = home;
	else
	{
		inPos = false;
		rightS->position = {pos2.x + dir2.x * 19.0f * DeltaTime(), pos2.y + dir2.y * 19.0f * DeltaTime()};
	}
	if (!inPos)
	{
		calc += DeltaTime();
		if (calc >= 0.333f)
		{
			rightS->position = home;
			leftS->position = home;
			calc = 0.0f;
			inPos = true;
		}
	}
	else
		calc = 0.0f;
	return (inPos);
}

void PlayerMovement::ShoesAnimation()
{
	static float x = 0.0f;
	float xAdd = (X_ADD / 6.5f) * fabs(direction.x);
	if (!pressed)
	{
		shoesInPosition = PositionShoes();
		firstCycle = true;
		x = 0.0f;
		return ;
	}
	if ((float)leftRight * direction.x < 0.0f || !shoesInPosition)
	{
		shoesInPosition = PositionShoes();
		firstCycle = true;
		x = 0.0f;
		return ;
	}
	ShoesWhilePressedF(x);
	ShoesWhilePressedB(x);
	x += xAdd * DeltaTime();
	if (x >= 1.0f)
	{
		firstCycle = false;
		x = 0.0f;
	}
}

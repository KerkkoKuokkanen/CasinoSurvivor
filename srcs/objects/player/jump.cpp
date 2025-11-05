
#include "player.h"
#include "keyboard.h"
#include "deltaTime.h"

#define TOP_PLAT 3.95f
#define BOTTOM_PLAT -6.7f

void PlayerMovement::AirMovement()
{
	if (position.x <= -9.3f || position.x >= -1.2f)
		inAir = true;
	if (inAir == false)
		return ;
	t_Point prew = face->position;
	leftS->position = torso->position;
	rightS->position = torso->position;
	direction.y -= 44.0f * DeltaTime();
	if (prew.y > BOTTOM_PLAT && position.y <= BOTTOM_PLAT && position.x > -9.3f && position.x < -1.2f)
	{
		jumped = 0;
		inAir = false;
		direction.y = 0.0f;
		position.y = BOTTOM_PLAT;
		return ;
	}
	if (prew.y > -1.35f && position.y <= -1.35f && position.x > -9.3f && position.x < -1.2f)
	{
		if (heldDown)
		{
			jumpBoost = 1.2f;
			jumped = 1;
			return ;
		}
		jumped = 0;
		inAir = false;
		direction.y = 0.0f;
		position.y = -1.35f;
		return ;
	}
	if (prew.y > TOP_PLAT && position.y <= TOP_PLAT && position.x > -9.3f && position.x < -1.2f)
	{
		if (heldDown)
		{
			jumpBoost = 1.2f;
			jumped = 1;
			return ;
		}
		jumped = 0;
		inAir = false;
		direction.y = 0.0f;
		position.y = TOP_PLAT;
		return ;
	}
}

void PlayerMovement::Jump()
{
	if (inAir && jumped == 1 && KeyPressed(SDL_SCANCODE_W))
	{
		inAir = true;
		jumped = 2;
		direction.y = 15.0f + jumpBoost;
		jumpBoost = 0.0f;
	}
	if (KeyPressed(SDL_SCANCODE_W) && fabs(direction.y) < 0.0001f)
	{
		jumped = 1;
		inAir = true;
		direction.y = 17.2f;
	}
	if (cyoteTime < 0.18f && KeyPressed(SDL_SCANCODE_W) && jumped == 0)
	{
		jumped = 1;
		inAir = true;
		direction.y = 15.0f;
	}
	else if (cyoteTime >= 0.18f && inAir && jumped == 0)
	{
		jumped = 1;
		if (KeyPressed(SDL_SCANCODE_W))
		{
			inAir = true;
			jumped = 2;
			direction.y = 15.0f + jumpBoost;
			jumpBoost = 0.0f;
		}
	}
	if (KeyHeld(SDL_SCANCODE_S) && position.y > BOTTOM_PLAT + 2.0f)
	{
		jumpBoost = 1.2f;
		inAir = true;
		cyoteTime = 10.0f;
		heldDown = true;
		if (fabs(direction.y) < 0.0001f)
			jumped = 1;
	}
	else
		heldDown = false;
	if (inAir)
	{
		cyoteTime += DeltaTime();
		if (cyoteTime > 1000.0f)
			cyoteTime = 1000.0f;
	}
	else
		cyoteTime = 0.0f;
}

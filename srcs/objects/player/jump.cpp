
#include "player.h"
#include "keyboard.h"
#include "deltaTime.h"

void PlayerMovement::AirMovement()
{
	if (inAir == false)
		return ;
	leftS->position = torso->position;
	rightS->position = torso->position;
	direction.y -= 44.0f * DeltaTime();
	if (position.y <= -1.35f)
	{
		inAir = false;
		direction.y = 0.0f;
		position.y = -1.35f;
	}
}

void PlayerMovement::Jump()
{
	if (KeyPressed(SDL_SCANCODE_W) && fabs(direction.y) < 0.0001f)
	{
		inAir = true;
		direction.y += 23.0f;
	}
}

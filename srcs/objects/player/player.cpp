
#include "player.h"
#include "keyboard.h"
#include "deltaTime.h"

#define MAX_SPEED 6.5f
#define START_X -5.0f
#define START_Y -1.35f

PlayerMovement::PlayerMovement()
{
	face = new Image("Face", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	glasses = new Image("glasses", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	hair = new Image("hair", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	torso = new Image("torso", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	leftH = new Image("handLeft", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	rightH = new Image("handRight", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	leftS = new Image("shoeLeft", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	rightS = new Image("shoeRight", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	face->drawDepth = 1.0f;
	glasses->drawDepth = 2.0f;
	hair->drawDepth = 1.5f;
	torso->drawDepth = 0.5f;
	leftH->drawDepth = 1.0f;
	rightH->drawDepth = 0.0f;
	leftS->drawDepth = 1.0f;
	rightS->drawDepth = 0.0f;
	position = {START_X, START_Y};
}

void PlayerMovement::GetInputs()
{
	float x = 0.0f;
	if (KeyHeld(SDL_SCANCODE_A))
	{
		x -= 30.0f;
		pressed = true;
		leftRight = -1;
	}
	if (KeyHeld(SDL_SCANCODE_D))
	{
		x += 30.0f;
		pressed = true;
		leftRight = 1;
	}
	if (direction.x * x < 0.0f)
		direction.x *= 0.8f;
	direction.x += x * DeltaTime();
	if (direction.x < -MAX_SPEED)
		direction.x = -MAX_SPEED;
	else if (direction.x > MAX_SPEED)
		direction.x = MAX_SPEED;
}

void PlayerMovement::MovePlayer()
{
	position.x += direction.x * DeltaTime();
	position.y += direction.y * DeltaTime();
	t_Point prew = face->position;
	t_Point diff = {position.x - prew.x, position.y - prew.y};
	AirMovement();
	if (fabs(diff.x) + fabs(diff.y) < 0.01f)
	{
		face->position = position;
		glasses->position = position;
		hair->position = position;
		leftH->position = position;
		rightH->position = position;
	}
	else
	{
		t_Point newPos = {prew.x + diff.x * 0.5f, prew.y + diff.y * 0.5f};
		face->position = newPos;
		glasses->position = newPos;
		hair->position = newPos;
		leftH->position = newPos;
		rightH->position = newPos;
	}
	torso->position = position;
	if (!pressed)
	{
		direction.x *= 0.9f;
		if (fabs(direction.x) < 0.1f)
			direction.x = 0.0f;
	}
}

void PlayerMovement::HeadAnimation()
{
	if (inAir)
		return ;
	static float x = 0.0f;
	float angle = -cos(x) / 10.0f - 0.08f;
	face->angle = angle;
	glasses->angle = angle / 1.5f;
	hair->angle = angle;
	torso->position.y += angle / 2.0f;
	leftH->position.y += angle / 3.0f;
	rightH->position.y += angle / 3.0f;
	leftH->position.x += angle / 4.0f;
	rightH->position.x += angle / 4.0f;
	x += 14.0f * DeltaTime();
	if (x > PI * 2)
		x = 0.0f;
}

void PlayerMovement::Update()
{
	leftRight = 0;
	pressed = false;
	GetInputs();
	Jump();
	MovePlayer();
	ShoesAnimation();
	HeadAnimation();
}

void PlayerMovement::Start()
{
	self->weight = -1.0f;
}

PlayerMovement::~PlayerMovement()
{
	if (face)
		delete face;
	if (hair)
		delete hair;
	if (glasses)
		delete glasses;
	if (torso)
		delete torso;
	if (leftH)
		delete leftH;
	if (rightH)
		delete rightH;
	if (leftS)
		delete leftS;
	if (rightS)
		delete rightS;
}

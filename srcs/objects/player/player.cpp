
#include "player.h"
#include "keyboard.h"
#include "deltaTime.h"
#include "envHandler.h"
#include <cmath>

#define MAX_SPEED 6.5f
#define START_X -5.0f
#define START_Y -1.35f
#define DAMPING std::pow(0.9, 60)

PlayerMovement::PlayerMovement()
{
	face = new Image("Face", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	glasses = new Image("glasses", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	hair = new Image("hair", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	torso = new Image("torso", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	leftS = new Image("shoeLeft", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	rightS = new Image("shoeRight", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	face->drawDepth = 1.0f;
	glasses->drawDepth = 2.0f;
	hair->drawDepth = 1.5f;
	torso->drawDepth = 0.5f;
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
		direction.x *= 0.8f * DeltaTime();
	direction.x += x * DeltaTime();
	if (direction.x < -MAX_SPEED)
		direction.x = -MAX_SPEED;
	else if (direction.x > MAX_SPEED)
		direction.x = MAX_SPEED;
}

void PlayerMovement::MovePlayer()
{
	position.x += direction.x * DeltaTime() + extraForces.x * DeltaTime();
	position.y += direction.y * DeltaTime() + extraForces.y * DeltaTime();
	t_Point prew = face->position;
	t_Point diff = {position.x - prew.x, position.y - prew.y};
	AirMovement();
	if (grid != NULL)
	{
		float sum = fabs(direction.x) + fabs(direction.y);
		grid->ApplyForce({position.x, position.y - 1.0f}, sum * 0.016f * DeltaTime(), 0.3f);
	}
	if (fabs(diff.x) + fabs(diff.y) < 0.01f)
	{
		face->position = position;
		glasses->position = position;
		hair->position = position;
	}
	else
	{
		t_Point newPos = {prew.x + diff.x * 0.5f, prew.y + diff.y * 0.5f};
		face->position = newPos;
		glasses->position = newPos;
		hair->position = newPos;
	}
	torso->position = position;
	if (!pressed)
	{
		direction.x *= std::pow(DAMPING, DeltaTime());
		if (fabs(direction.x) < 0.1f)
			direction.x = 0.0f;
	}
	extraForces.x *= std::pow(DAMPING, DeltaTime());
	extraForces.y *= std::pow(DAMPING, DeltaTime());
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
	x += 14.0f * DeltaTime();
	if (x > PI * 2)
		x = 0.0f;
}

void PlayerMovement::CameraMovement()
{
	if (cam == NULL)
		return ;
	t_Point curPos = cam->GetCameraPosition();
	if (position.y > 6.8f)
		curPos.y = fmin((position.y - 6.8f) / (position.y * 0.19f), 2.0f);
	else
		curPos.y = 0.0f;
	if (position.x < -8.6f)
		curPos.x = fmax(-2.0f, (position.x + 8.6f) / (fabs(position.x) * 0.17f));
	else
		curPos.x = 0.0f;
	cam->SetCameraPosition(curPos.x, curPos.y);
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
	CameraMovement();
}

void PlayerMovement::Start()
{
	componentWeight = -1.0f;
	SystemObj *obj = FindSystemObject(17206662188527305259LU);
	if (obj)
		grid = (FloorGrid*)obj->GetComponent("FloorGrid");
	obj = FindSystemObject(4874219225160765108LU);
	if (obj)
		cam = (Camera*)obj->GetComponent("Camera");
}

void PlayerMovement::ApplyXForce(float force)
{
	extraForces.x += force;
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
	if (leftS)
		delete leftS;
	if (rightS)
		delete rightS;
}

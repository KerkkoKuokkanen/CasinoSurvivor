
#include "player.h"
#include "keyboard.h"
#include "deltaTime.h"
#include "envHandler.h"
#include "vectorTools.h"
#include "audio.h"
#include <cmath>

#define MAX_SPEED 6.5f
#define START_X -5.0f
#define START_Y -0.25f
#define DAMPING std::pow(0.9, 60)
#define INVIS_TIME 1.0f

PlayerMovement::PlayerMovement()
{
	face = new Image("Face", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	glasses = new Image("glasses", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	hair = new Image("hair", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	torso = new Image("torso", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	leftS = new Image("shoeLeft", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	rightS = new Image("shoeRight", {START_X, START_Y, 3.6f, 3.6f}, 0.0f, 5);
	pill = new Image("pill2", {9.32f, -5.0f, 1.1f, 1.1f}, 0.0f, 13);
	pill->SetTransformType(n_TransformTypes::TRANSFORM_STATIC);
	pill->drawDepth = 9999.0f;
	face->drawDepth = 1.0f;
	glasses->drawDepth = 2.0f;
	hair->drawDepth = 1.5f;
	torso->drawDepth = 0.5f;
	leftS->drawDepth = 1.0f;
	rightS->drawDepth = 0.0f;
	position = {START_X, START_Y};
	num = new Number({8.28f, -5.0f}, std::to_string(health), 0.7f, {0.8f, 0.8f, 0.8f, 1.0f}, 9000.0f);
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
	position.x += direction.x * DeltaTime() + extraForce * DeltaTime();
	position.y += direction.y * DeltaTime();
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
	extraForce *= std::pow(DAMPING, DeltaTime());
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
	if (position.y > 3.825f)
		curPos.y = fmin((position.y - 3.825f) / (position.y * 0.19f), 1.65f);
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
	NumAnim();
	CameraMovement();
	DamageAnimation();
	hitBox = {position.x - 0.3f, position.y - 0.8f, 0.8f, 1.6f};
	if (invisTime >= 0.0f)
		invisTime -= DeltaTime();
	else
		canHit = true;
}

void PlayerMovement::DamageAnimation()
{
	if (invisTime <= 0.0f)
	{
		face->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		hair->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		torso->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		leftS->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		rightS->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		return ;
	}
	if (damageTime <= 0.0f)
	{
		damageTime = 0.1f;
		if (cycle)
		{
			face->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			hair->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			torso->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			leftS->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			rightS->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			cycle = false;
		}
		else
		{
			face->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
			hair->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
			torso->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
			leftS->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
			rightS->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
			cycle = true;
		}
	}
	damageTime -= DeltaTime();
}

void PlayerMovement::Damage(int damage, float strength, t_Point pos)
{
	soundKey[0] = RePlaySound("hit1", 25.0f, 0, soundKey[0]);
	soundKey[1] = RePlaySound("hit2", 5.0f, 0, soundKey[1]);
	soundKey[2] = RePlaySound("hit3", 30.0f, 0, soundKey[2]);
	soundKey[3] = RePlaySound("death2", 30.0f, 0, soundKey[3]);
	numAddTime = 0.6f / ((float)damage);
	health += damage;
	float mult = fmax((float)health / 10.0f, 1.0f);
	ApplyXForce(-strength * mult);
	if (pos.y < position.y)
	{
		inAir = true;
		if (jumped == 0)
			jumped = 1;
		jumpBoost = 0.0f;
		float m = fmin(strength * 0.25f * mult, 12.0f);
		direction.y = m;
	}
	invisTime = INVIS_TIME;
	canHit = false;
	face->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	hair->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	torso->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	leftS->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	rightS->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	cycle = true;
	damageTime = 0.1f;
}

void PlayerMovement::NumAnim()
{
	if (num->num == health)
		return ;
	if (numberAddTime >= numAddTime)
	{
		int amount = rounding(numberAddTime / numAddTime);
		int n = num->num + amount;
		if (n > health)
			n = health;
		delete num;
		std::string nn = std::to_string(n);
		float posX = 8.28f - (nn.length() - 1) * 0.7f;
		num = new Number({posX, -5.0f}, nn, 0.7f, {0.8f, 0.8f, 0.8f, 1.0f}, 9000.0f);
		numberAddTime = 0.0f;
	}
	numberAddTime += DeltaTimeReal();
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
	extraForce += force;
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
	if (pill)
		delete pill;
	if (num)
		delete num;
}

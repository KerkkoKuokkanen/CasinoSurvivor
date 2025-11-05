
#include "pistol.h"
#include "player.h"
#include "mouse.h"
#include "vectorTools.h"
#include "deltaTime.h"
#include "commonTools.h"
#include "audio.h"

Pistol::Pistol()
{
	pistol = new Image("pistolWHands", {0.0f, 0.0f, 1.1f, 1.1f}, 0.0f, 5);
	pistol->drawDepth = 20.0f;
	pistol->SetColor(0.7f, 0.7f, 0.7f, 1.0f);
	hand1 = new Image("pistolHL", {0.0f, 0.0f, 1.1f, 1.1f}, 0.0f, 5);
	hand1->drawDepth = 21.0f;
	hand2 = new Image("pistolHR", {0.0f, 0.0f, 1.1f, 1.1f}, 0.0f, 5);
	hand2->drawDepth = 22.0f;
}

Pistol::~Pistol()
{
	if (pistol != NULL)
		delete pistol;
	if (hand1 != NULL)
		delete hand1;
	if (hand2 != NULL)
		delete hand2;
}

void Pistol::Start()
{
	player = (PlayerMovement*)self->GetComponent("PlayerMovement");
}

void Pistol::PositionPistol()
{
	t_Point position = player->GetPosition();
	position.x += 1.25f;
	position.y += 0.5f;
	pistol->position = position;
	hand1->position = position;
	hand2->position = position;
}

void Pistol::PistolAnglePosition()
{
	t_Point pos = pistol->position;
	float angle = pistol->angle;
	float multi = (angle >= 0.0f) ? 1.0f : -1.0f;
	float x = fabs(angle);
	float y = -(angle * angle) + PI * fabs(angle);
	pos.x -= x * 0.4f;
	pos.y += y * multi * 0.5f;
	pistol->position = pos;
	hand1->position = pos;
	hand2->position = pos;
}

t_Point Pistol::GetMousePoint()
{
	float r = 6.0f;
	t_Point mouse = GetMouseXY();
	t_Point position = player->GetPosition();
	t_Point dir = {mouse.x - position.x, mouse.y - position.y};
	if (fabs(dir.x) + fabs(dir.y) < 0.6f)
		return (t_Point){r, 0.0f};
	dir = VectorNormalize(dir);
	if (dir.x < 0.0f)
		dir.x = -dir.x;
	dir.x = position.x + dir.x * r;
	dir.y = position.y + dir.y * r;
	return (dir);
}

bool Pistol::AnglePistol()
{
	t_Point mouse = GetMousePoint();
	t_Point pos = pistol->position;
	pos.x -= 0.5f;
	pos.y += 0.3f;
	t_Point dir = {mouse.x - pos.x, mouse.y - pos.y};
	dir = VectorNormalize(dir);
	shootDir = dir;
	float angle = VectorAngle({1.0f, 0.0f}, dir);
	angle = fmin(fabs(angle), PI * 0.5f);
	if (mouse.y < pos.y)
		angle = -angle;
	pistol->angle = angle;
	hand1->angle = angle;
	hand2->angle = angle;
	return (true);
}

void Pistol::GunBopping()
{
	static float x = 0.0f;
	x += 14.0f * DeltaTime();
	if (x > PI * 2)
		x = 0.0f;
	if (player->inAir || shooting)
		return ;
	float diff = -cos(x) / 16.0f - 0.08f;
	pistol->position.y += diff;
	hand1->position.y += diff;
	hand2->position.y += diff;
}

void Pistol::FullPosition()
{
	PositionPistol();
	if (AnglePistol())
		PistolAnglePosition();
	AnglePistol();
}

//2.3415
void Pistol::ShootingAnimation()
{
	if (cycle < 2.3415)
		shooting = true;
	else
	{
		FullPosition();
		return ;
	}
	FullPosition();
	float val = sin(cycle + 0.8f);
	t_Point pos = pistol->position;
	pos.x += shootingForce.x * val;
	pos.y += shootingForce.y * val;
	pistol->angle += 0.8f * val;
	hand1->angle += 0.8f * val;
	hand2->angle += 0.8f * val;
	pistol->position = pos;
	hand1->position = pos;
	hand2->position = pos;
	cycle += 15.0f * DeltaTime();
}

void Pistol::Shooting()
{
	if (recharge > 0.0f)
		recharge -= DeltaTime();
	if (MouseKeyHeld(n_MouseKeys::MOUSE_LEFT))
		shooting = true;
	if (!shooting)
		return ;
	shooting = false;
	if (MouseKeyHeld(n_MouseKeys::MOUSE_LEFT) && recharge <= 0.0f)
	{
		audioKey = RePlaySound("pistol", 10.0f, 0, audioKey);
		FullPosition();
		t_Point dir = VectorRotate({1.0f, 0.0f}, pistol->angle);
		float multi = 0.6f + float_rand() * 0.1f;
		shootingForce = {-dir.x * multi, -dir.y * multi};
		recharge = fireRate;
		cycle = 0.0f;
	}
	ShootingAnimation();
}

void Pistol::Update()
{
	if (!shooting)
		FullPosition();
	Shooting();
	GunBopping();
}

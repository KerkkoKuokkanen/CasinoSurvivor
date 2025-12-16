
#include "pistol.h"
#include "player.h"
#include "mouse.h"
#include "vectorTools.h"
#include "deltaTime.h"
#include "commonTools.h"
#include "audio.h"
#include "envHandler.h"

//153, 153, 153
//102, 102, 102
//181, 181, 181
//46, 46, 46
//227, 122, 30
//227, 150, 27
Pistol::Pistol()
{
	colors.push_back({0.6f, 0.6f, 0.6f, 1.0f});
	colors.push_back({0.4f, 0.4f, 0.4f, 1.0f});
	colors.push_back({0.71f, 0.71f, 0.71f, 1.0f});
	colors.push_back({0.18f, 0.18f, 0.18f, 1.0f});
	colors.push_back({0.89f, 0.48f, 0.12f, 1.0f});
	colors.push_back({0.89f, 0.59f, 0.11f, 1.0f});
	pistol = new Image("pistolJust", {-5.0f, 0.0f, 1.5f, 1.5f}, 0.0f, 5);
	pistol->drawDepth = 20.0f;
	pistol->SetColor(0.7f, 0.7f, 0.7f, 1.0f);
	hand1 = new Image("pistolHL", {-5.0f, 0.0f, 1.5f, 1.5f}, 0.0f, 5);
	hand1->drawDepth = 22.0f;
	hand2 = new Image("pistolHR", {-5.0f, 0.0f, 1.5f, 1.5f}, 0.0f, 5);
	hand2->drawDepth = 21.0f;
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
	SystemObj *obj = FindSystemObject(17206662188527305259LU);
	if (obj)
		grid = (FloorGrid*)obj->GetComponent("FloorGrid");
	player = (PlayerMovement*)self->GetComponent("PlayerMovement");
	obj = FindSystemObject(6302736476082374709LU);
	if (obj)
		bullets = (BulletManager*)obj->GetComponent("BulletManager");
	obj = FindSystemObject(10158550851708924347LU);
	parts = (Particles*)obj->GetComponent("Particles");
}

void Pistol::PositionPistol()
{
	t_Point position = player->GetPosition();
	position.x += 1.0f;
	position.y += 0.34f;
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
	pos.y += y * multi * 0.26f;
	pistol->position = pos;
	hand1->position = pos;
	hand2->position = pos;
}

t_Point Pistol::GetMousePoint()
{
	float r = 2.0f;
	t_Point mouse = GetMouseXY();
	t_Point position = player->GetPosition();
	t_Point dir = {mouse.x - position.x, mouse.y - position.y};
	if (fabs(dir.x) + fabs(dir.y) < 0.6f)
		return (t_Point){r, 0.0f};
	float mag = VectorMagnitude(dir);
	if (mag > r)
		r = mag;
	dir.x /= mag;
	dir.y /= mag;
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

void Pistol::CreateBullet()
{
	t_Point pos = pistol->position;
	t_Point mouse = GetMousePoint();
	t_Point dir = {mouse.x - pos.x, mouse.y - pos.y};
	if (mouse.x <= pos.x)
		dir.x = 0.0f;
	dir = VectorNormalize(dir);
	t_Point bulletPos = {pos.x + dir.x * 0.5f, pos.y + dir.y * 0.5f};
	recoil = fmax(recoil, 0.0f);
	recoil = (recoil > 0.3f) ? 0.3f : recoil;
	float usedRec = recoil * 0.5f;
	float angle = (rand() % 2 == 0) ? -usedRec * float_rand() : usedRec * float_rand();
	dir = VectorRotate(dir, angle);
	if (bullets)
	{
		parts->AddParticles({bulletPos.x - 0.25f, bulletPos.y}, 0.1f, dir, 0.3f, colors, 3.0f, 6.0f, 20, "everyColor", 0.025f, 0.05f, 0.12f, 0.18f, 0.0f);
		bullets->CreateBullet(bulletPos, dir, 10.0f, {0.76f, 0.53f, 0.25f, 1.0f});
	}
	recoil = recoil + 0.4f;
}

void Pistol::Shooting()
{
	if (recoil > 0.0f)
		recoil -= DeltaTime();
	if (recharge > 0.0f)
		recharge -= DeltaTime();
	if (MouseKeyHeld(n_MouseKeys::MOUSE_LEFT))
		shooting = true;
	if (!shooting)
		return ;
	shooting = false;
	if (MouseKeyHeld(n_MouseKeys::MOUSE_LEFT) && recharge <= 0.0f)
	{
		player->ApplyXForce(-4.2f);
		audioKey = RePlaySound("pistol", 10.0f, 0, audioKey);
		FullPosition();
		if (grid)
			grid->ApplyForce(pistol->position, 0.05f, 0.3f);
		t_Point dir = VectorRotate({1.0f, 0.0f}, pistol->angle);
		float multi = 0.6f + float_rand() * 0.1f;
		shootingForce = {-dir.x * multi, -dir.y * multi};
		recharge = fireRate;
		cycle = 0.0f;
		CreateBullet();
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


#include "cherry.h"
#include "deltaTime.h"
#include "commonTools.h"
#include "envHandler.h"
#include "particles.h"

#define ATTACK_TIME 0.4f
#define ATTACK_MINI_TIME 0.1f

Cherry::~Cherry()
{
	if (img != NULL)
		delete img;
	if (death != NULL)
		delete death;
}

Cherry::Cherry(t_Point pos, float size, EnemyData data)
{
	type = 1;
	health = data.health;
	Cherry::size = size;
	hbSize = size * 0.33f;
	hbSize2 = hbSize * 2.0f;
	img = new Image("EnemySheet", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 7);
	img->SetTextureData(0.37f, 0.0f, 0.16f, 0.315f, 0.0f);
	img->dimentions.x = size;
	img->dimentions.y = size;
	Position(pos.x, pos.y);
	cycleMag = float_rand() * 0.005f + 0.01f;
	cycle = float_rand() * PI;
	death = new Image("EnemySheetWhite", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 7);
	death->SetTextureData(0.37f, 0.0f, 0.16f, 0.315f, 0.0f);
	death->drawDepth = -1.0f;
	death->drawActive = false;
	death->SetColor(0.63f, 0.08f, 0.08f, 0.9f);
}

void Cherry::Position(float x, float y)
{
	hitbox.x = x - hbSize;
	hitbox.y = y - hbSize;
	hitbox.w = hbSize2;
	hitbox.h = hbSize2;
	position = {x, y};
	img->position = {x, y};
}

void Cherry::UpdateDeath()
{
	if (deathTime <= 0.0f)
	{
		active = false;
		return ;
	}
	float a = DeltaTime() * 2.0f;
	float b = 3.0f * a;
	death->dimentions = {death->dimentions.x * (1.0f + b), death->dimentions.y * (1.0f + b)};
	img->dimentions = {img->dimentions.x * (1.0f + 2.0f * b), img->dimentions.y * (1.0f - b)};
	t_Box col1 = img->GetColor();
	img->SetColor(1.0f, 1.0f, 1.0f, col1.h - a);
	t_Box col2 = death->GetColor();
	death->SetColor(0.63f, 0.08f, 0.08f, col2.h - a);
	deathTime -= DeltaTime();
}

void Cherry::UpdateAttack()
{
	if (attackTime <= 0.0f)
	{
		death->drawActive = false;
		return ;
	}
	if (attackMiniTime <= 0.0f)
	{
		attackMiniTime = ATTACK_MINI_TIME;
		float xr = 0.05f * multi;
		float yr = 0.05f * -multi;
		multi *= -1.0f;
		Position(position.x + xr, position.y + yr);
	}
	float scale = (size * 1.5f - size) / ATTACK_TIME;
	float ns = scale * attackTime;
	death->position = {img->position.x, img->position.y};
	death->angle = img->angle;
	death->dimentions = {size + ns, size + ns};
	attackTime -= DeltaTimeReal();
	attackMiniTime -= DeltaTimeReal();
}

void Cherry::Update()
{
	if (dead)
		return (UpdateDeath());
	Position(position.x, position.y + sin(cycle) * cycleMag);
	img->angle = sin(cycle) * 0.2f;
	cycle += DeltaTime() * 4.0f;
	if (cycle >= PI * 2.0f)
		cycle = 0.0f;
	if (color.x < 1.0f)
	{
		color.x += DeltaTime() * 2.0f;
		color.y += DeltaTime() * 2.0f;
		color.w += DeltaTime() * 2.0f;
	}
	if (color.x > 1.0f)
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.w = 1.0f;
	}
	UpdateAttack();
	img->SetColor(color.x, color.y, color.w, 1.0f);
}

//135, 24, 12
//0.63f, 0.08f, 0.08f
//255, 48, 25
//74, 21, 15
//227, 105, 91
void Cherry::KillEnemy()
{
	dead = true;
	death->drawActive = true;
	death->dimentions = {size, size};
	death->position = {img->position.x, img->position.y};
	death->angle = img->angle;
}

void Cherry::Attack()
{
	attackTime = ATTACK_TIME;
	death->drawActive = true;
	death->dimentions = {size * 1.5f, size * 1.5f};
	death->position = {img->position.x, img->position.y};
	death->angle = img->angle;
	attackMiniTime = ATTACK_MINI_TIME;
}

void Cherry::EnemyHit(int damage, t_Point pos, unsigned int name)
{
	img->SetColor(0.4f, 0.4f, 0.4f, 1.0f);
	color = {0.4f, 0.4f, 0.4f, 1.0f};
}

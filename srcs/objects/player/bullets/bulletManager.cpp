
#include "bulletManager.h"
#include "enemySpawner.h"
#include "envHandler.h"
#include "audio.h"

#define BULLET_LIFE_TIME 4.0f

static bool HitBoxCheck(t_Box one, t_Box two)
{
	bool x = (one.x + one.w > two.x) && (one.x < two.x + two.w);
	bool y = (one.y + one.h > two.y) && (one.y < two.y + two.h);
	return (x && y);
}

static unsigned int BulletNamer()
{
	static unsigned int name = 0;
	name += 1;
	if (name > 4294967290)
		name = 0;
	return (name);
}

BulletManager::BulletManager()
{
	numbers.reserve(MAX_NUMBERS);
}

BulletManager::~BulletManager()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bullets[i].img != NULL)
			delete bullets[i].img;
	}
	for (int i = 0; i < numbers.size(); i++)
	{
		delete numbers[i].num;
	}
}

void BulletManager::Start()
{
	SystemObj *obj = FindSystemObject(17206662188527305259LU);
	grid = (FloorGrid*)obj->GetComponent("FloorGrid");
	shaker = (CameraShake*)self->GetComponent("CameraShake");
	obj = FindSystemObject(10158550851708924347LU);
	part = (Particles*)obj->GetComponent("Particles");
	chips = (ChipManager*)self->GetComponent("ChipManager");
	std::vector<t_Box> col1;
	col1.push_back({0.63f, 0.08f, 0.08f, 1.0f});
	col1.push_back({0.53f, 0.09f, 0.045f, 1.0f});
	col1.push_back({1.0f, 0.19f, 0.1f, 1.0f});
	col1.push_back({0.3f, 0.08f, 0.06f, 1.0f});
	col1.push_back({0.9f, 0.4f, 0.4f, 1.0f});
	colors.push_back(col1);
	componentWeight = -10.0f;
}

int BulletManager::FindFreeIndex()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bullets[i].active == false)
			return (i);
	}
	return (-1);
}

void BulletManager::CreateBullet(t_Point pos, t_Point dir, float speed, t_Box color)
{
	int index = FindFreeIndex();
	if (index == (-1))
		return ;
	bullets[index].dir = dir;
	bullets[index].pos = pos;
	bullets[index].lifeTime = BULLET_LIFE_TIME;
	if (bullets[index].img == NULL)
		bullets[index].img = new Image("Bullet1", {0.0f, 0.0f, 0.4f, 0.4f}, 0, 9);
	else
		bullets[index].img->SetTexture("Bullet1");
	bullets[index].img->position = pos;
	bullets[index].img->drawActive = true;
	bullets[index].active = true;
	bullets[index].speed = speed;
	bullets[index].img->SetColor(color.x, color.y, color.w, color.h);
	bullets[index].name = BulletNamer();
	bullets[index].size = 0.1f;
	bullets[index].damage = 4;
	bullets[index].pierce = 1;
	bullets[index].multiHit = false;
}

bool BulletManager::bulletOutOfBound(t_Point pos)
{
	if (pos.x > 18.0f)
		return (true);
	if (pos.x < -18.0f)
		return (true);
	if (pos.y > 9.0f)
		return (true);
	if (pos.y < -9.0f)
		return (true);
	return (false);
}

void BulletManager::CreateDamageNumber(t_Point pos, int damage)
{
	if (numbers.size() >= MAX_NUMBERS)
		return ;
	t_Box color = {1.0f, 0.82f, 0.31f, 1.0f};
	Number *num = new Number({pos.x + 0.05f, pos.y + 0.15f}, std::to_string(damage), 0.33f, color, numDepth, false);
	numDepth += 0.01f;
	if (numDepth > 25000.0f)
		numDepth = 0.0f;
	numbers.push_back({num, color, 0.3f, false});
}

bool BulletManager::CheckEnemyHit(std::vector<CommonEnemy*> &enemies, int index)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		int ret = enemies[i]->ApplyDamage(bullets[index].pos, bullets[index].size, bullets[index].damage, bullets[index].name);
		if (ret == 0)
			continue ;
		if (ret == 1)
		{
			grid->ApplyForce(enemies[i]->position, 0.05f, 0.2f);
			enemies[i]->EnemyHit(bullets[index].damage, bullets[index].pos, bullets[index].name);
			CreateDamageNumber(enemies[i]->position, bullets[index].damage);
		}
		else if (ret == 2)
		{
			soundKey3 = RePlaySound("death1", 4.5f, 0, soundKey3);
			grid->ApplyForce(enemies[i]->position, 0.1f, 0.4f);
			shaker->CreateCameraShake(0.05f);
			CreateDeathParticles(enemies[i]);
			enemies[i]->KillEnemy();
			CreateDamageNumber(enemies[i]->position, bullets[index].damage);
			chips->CreateChips(enemies[i]->position, enemies[i]->currency);
		}
		bullets[index].active = false;
		bullets[index].img->drawActive = false;
		soundKey1 = RePlaySound("death2", 8.0f, 0, soundKey1);
		soundKey2 = RePlaySound("Hitmark5", 3.5f, 0, soundKey2);
		return (true);
	}
	return (false);
}

static bool CheckNumTime(DamageNumber num)
{
	return num.done;
}

void BulletManager::UpdateDamageNumbers()
{
	float scale = 1.0f / 0.15f;
	for (int i = 0; i < numbers.size(); i++)
	{
		t_Point pos = numbers[i].num->GetPosition();
		numbers[i].num->SetPosition({pos.x, pos.y + numbers[i].time * DeltaTime() * 6.0f});
		float alpha = scale * fmax(numbers[i].time, 0.0f);
		t_Box col = numbers[i].color;
		numbers[i].num->SetColor(col.x, col.y, col.w, fmin(1.0f, alpha));
		numbers[i].time -= DeltaTime();
		if (numbers[i].time <= 0.0f)
		{
			numbers[i].done = true;
			delete numbers[i].num;
		}
	}
	numbers.erase(std::remove_if(numbers.begin(), numbers.end(), CheckNumTime), numbers.end());
}

void BulletManager::Update()
{
	UpdateDamageNumbers();
	EnemySpawner *spwn = (EnemySpawner*)self->GetComponent("EnemySpawner");
	if (spwn == NULL)
		return ;
	std::vector<CommonEnemy*> enemies = spwn->GetEnemies();
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (!bullets[i].active)
			continue ;
		bullets[i].lifeTime -= DeltaTime();
		if (bullets[i].lifeTime <= 0.0f)
		{
			bullets[i].img->drawActive = false;
			bullets[i].active = false;
			continue ;
		}
		if (bulletOutOfBound(bullets[i].pos))
		{
			bullets[i].img->drawActive = false;
			bullets[i].active = false;
			continue ;
		}
		bullets[i].pos.x += bullets[i].dir.x * bullets[i].speed * DeltaTime();
		bullets[i].pos.y += bullets[i].dir.y * bullets[i].speed * DeltaTime();
		bullets[i].img->position = bullets[i].pos;
		CheckEnemyHit(enemies, i);
	}
}

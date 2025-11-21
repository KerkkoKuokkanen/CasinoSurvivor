
#include "bulletManager.h"
#include "deltaTime.h"

#define BULLET_LIFE_TIME 4.0f

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

}

BulletManager::~BulletManager()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bullets[i].img != NULL)
			delete bullets[i].img;
	}
}

void BulletManager::Start()
{
	componentWeight = 10.0f;
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
	bullets[index].size = 0.3f;
	bullets[index].damage = 4;
}

bool BulletManager::bulletOutOfBound(t_Point pos)
{
	if (pos.x > 14.0f)
		return (true);
	if (pos.x < -18.0f)
		return (true);
	if (pos.y > 15.0f)
		return (true);
	if (pos.y < -15.0f)
		return (true);
	return (false);
}

void BulletManager::Update()
{
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
	}
}


#include "bulletManager.h"
#include "deltaTime.h"

#define BULLET_LIFE_TIME 4.0f
#define BULLET_SPEED 3.0f

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
	self->weight = 10.0f;
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

void BulletManager::CreateBullet(t_Point pos, t_Point dir)
{
	int index = FindFreeIndex();
	if (index == (-1))
		return ;
	bullets[index].dir = dir;
	bullets[index].pos = pos;
	bullets[index].lifeTime = BULLET_LIFE_TIME;
	if (bullets[index].img == NULL)
		bullets[index].img = new Image("bullet1", {0.0f, 0.0f, 0.4f, 0.4f}, 0, 9);
	else
		bullets[index].img->SetTexture("bullet1");
	bullets[index].img->position = pos;
	bullets[index].active = true;
}

void BulletManager::Update()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bullets[i].lifeTime -= DeltaTime();
		if (bullets[i].lifeTime <= 0.0f)
		{
			bullets[i].img->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
			bullets[i].active = false;
			continue ;
		}
		bullets[i].pos.x += bullets[i].dir.x;
		bullets[i].pos.y += bullets[i].dir.y;
		bullets[i].img->position = bullets[i].pos;
	}
}

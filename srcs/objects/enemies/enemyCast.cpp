
#include "enemySpawner.h"
#include "cherry.h"

#define SPAWN_AREA_X 11.0f
#define SPAWN_AREA_X_WIDTH 7.0f
#define SPAWN_AREA_Y -5.2f
#define SPAWN_AREA_Y_HEIGHT 12.6f
#define MAX_ITERATIONS 100

static bool HitBoxCheck(t_Box one, t_Box two)
{
	bool x = (one.x + one.w > two.x) && (one.x < two.x + two.w);
	bool y = (one.y + one.h > two.y) && (one.y < two.y + two.h);
	return (x && y);
}

std::tuple<t_Point, float, bool> EnemySpawner::FindPosAndSize(EnemyData &data)
{
	float size = (data.size + float_rand() * 0.5f) * 0.5f;
	float hb = size * 0.33f;
	float db = hb * 2.0f;
	for (int i = 0; i < MAX_ITERATIONS; i++)
	{
		float x = float_rand() * SPAWN_AREA_X_WIDTH + SPAWN_AREA_X;
		float y = float_rand() * SPAWN_AREA_Y_HEIGHT + SPAWN_AREA_Y;
		t_Box area = {x - hb, y - hb, db, db};
		bool hit = false;
		CommonEnemy *p = NULL;
		for (CommonEnemy *e : enemies)
		{
			p = e;
			if (HitBoxCheck(e->hitbox, area))
			{
				hit = true;
				break ;
			}
		}
		if (!hit)
			return {{x, y}, size, true};
	}
	return {{0.0f, 0.0f}, 0.0f, false};
}

void EnemySpawner::EnemyCasting(EnemyData &data, t_Point pos, float size)
{
	switch (data.type)
	{
		case 1:
			enemies.push_back(new Cherry(pos, size, data));
			break ;
		default :
			break ;
	}
}

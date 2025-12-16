
#include "bulletManager.h"
#include "envHandler.h"

void BulletManager::CreateDeathParticles(CommonEnemy *e)
{
	switch (e->type)
	{
		case 1:
		{
			part->AddParticles(e->position, 1.0f, {1.0f, 0.0f}, 2.0f * PI, colors[0], 1.5f, 8.0f, 40, "everyColor", 0.04, 0.06, 0.3f, 0.4f, 0.0f);
			break ;
		}
	}
}


#ifndef ENEMY_DATA_H
# define ENEMY_DATA_H

#include <stdlib.h>

struct EnemyData
{
	uint8_t type;
	int health;
	float speed;
	double time;
	float size;
	bool independent;
};

#endif

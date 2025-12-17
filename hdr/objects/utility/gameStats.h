
#ifndef GAME_STATS_H
# define GAME_STATS_H

//This file should only include numbers representing the stats
//No objects or allocation

struct StatBoosters {
	float fireRate;
	float armor;
	float robustness;
	float damage;
	float speed;
	float airSpeed;
	float econony;
	float gameSpeed;
};

void SetStats(StatBoosters st);
StatBoosters GetStats();

void SetMoney(int m);
int GetMoney();

void SetHealth(int h);
int GetHealth();

void ResetStats();

#endif


#include "gameStats.h"

//This file should only include numbers representing the stats
//No objects or allocation

static int health = 0;
static int money = 0;
static StatBoosters stats = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

void SetStats(StatBoosters st) {
	stats = st;
}

StatBoosters GetStats() {
	return (stats);
}

void SetMoney(int m) {
	money = m;
	if (money < 0)
		money = 0;
}

int GetMoney() {
	return (money);
}

void SetHealth(int h) {
	health = h;
}

int GetHealth() {
	return (health);
}

void ResetStats()
{
	health = 0;
	money = 0;
	stats = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
}

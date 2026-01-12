
#ifndef COMMON_ENEMY_H
# define COMMON_ENEMY_H

# include "box.h"
# include "enemyData.h"
# include <vector>

class CommonEnemy
{
	public:
		bool active = true;
		bool independent = false;
		int health = 0;
		int type;
		float speed = 1.0f;
		float size = 0.0f;
		int damage = 0;
		int currency = 0;
		float deathForce = 0.1f;
		float deathForceRadius = 0.4f;
		t_Box hitbox = {0.0f, 0.0f, 0.0f, 0.0f};
		t_Point position = {0.0f, 0.0f};
		std::vector<unsigned int> knownBullets = {};
		// 0 -> no hit
		// 1 -> hit
		// 2 -> dead
		int ApplyDamage(t_Point pos, float size, int damage, unsigned int name, bool multiHit = false);
		virtual void KillEnemy() {};
		virtual void Update() {};
		virtual void Position(float x, float y) {};
		virtual void EnemyHit(int damage, t_Point pos, unsigned int name) {};
		virtual void Attack() {};
		virtual ~CommonEnemy() {};
};

#endif

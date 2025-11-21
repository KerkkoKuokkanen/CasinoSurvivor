
#ifndef COMMON_ENEMY_H
# define COMMON_ENEMY_H

# include "box.h"
# include <vector>

class CommonEnemy
{
	protected:
		int health = 0;
		t_Box hitbox = {0.0f, 0.0f, 0.0f, 0.0f};
		std::vector<unsigned int> knownBullets = {};
	public:
		// 0 -> no hit
		// 1 -> hit
		// 2 -> dead
		int ApplyDamage(t_Point pos, float size, int damage, unsigned int name);
		virtual void KillEnemy() {};
};

#endif

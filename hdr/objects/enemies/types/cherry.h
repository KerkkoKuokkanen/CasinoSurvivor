
#ifndef CHERRY_H
# define CHERRY_H

#include "commonEnemy.h"
#include "image.h"

class Cherry : public CommonEnemy
{
	private:
		bool dead = false;
		Image *img = NULL;
		Image *death = NULL;
		float hbSize = 0.0f;
		float hbSize2 = 0.0f;
		float cycle = 0.0f;
		float cycleMag = 0.0f;
		t_Box color = {1.0f, 1.0f, 1.0f, 1.0f};
		float deathTime = 0.2f;
		float attackTime = 0.0f;
		float attackMiniTime = 0.0f;
		float multi = 1.0f;
		void UpdateDeath();
		void UpdateAttack();
	public:
		Cherry(t_Point pos, float size, EnemyData data);
		~Cherry();
		void Update() override;
		void Position(float x, float y) override;
		void EnemyHit(int damage, t_Point pos, unsigned int name) override;
		void KillEnemy() override;
		void Attack() override;
};

#endif

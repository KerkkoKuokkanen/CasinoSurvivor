
#ifndef BULLET_MANAGER_H
# define BULLET_MANAGER_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "commonEnemy.h"
# include "image.h"
# include "camShake.h"
# include "floorGrid.h"
# include "particles.h"
# include "number.h"
# include "chipManager.h"

# define MAX_BULLETS 5000
# define MAX_NUMBERS 3000

struct Bullet {
	Image *img;
	t_Point pos;
	t_Point dir;
	float lifeTime;
	bool active;
	float speed;
	unsigned int name;
	float size;
	int damage;
	int pierce;
	bool multiHit;
};

struct DamageNumber {
	Number *num;
	t_Box color;
	float time;
	bool done;
};

class BulletManager : public CustomComponent
{
	private:
		std::vector<std::vector<t_Box>> colors;
		std::vector<DamageNumber> numbers;
		float numDepth = 0.0f;
		uint64_t soundKey1 = 0;
		uint64_t soundKey2 = 0;
		uint64_t soundKey3 = 0;
		Bullet bullets[MAX_BULLETS] = {0};
		CameraShake *shaker = NULL;
		FloorGrid *grid = NULL;
		Particles *part = NULL;
		ChipManager *chips = NULL;
		int FindFreeIndex();
		bool bulletOutOfBound(t_Point pos);
		bool CheckEnemyHit(std::vector<CommonEnemy*> &enemies, int index);
		void CreateDeathParticles(CommonEnemy *e);
		void CreateDamageNumber(t_Point pos, int damage);
		void UpdateDamageNumbers();
	public:
		BulletManager();
		~BulletManager();
		void Start() override;
		void CreateBullet(t_Point pos, t_Point dir, float speed, t_Box color);
		void Update() override;
};

REGISTER_COMPONENT(BulletManager);

#endif

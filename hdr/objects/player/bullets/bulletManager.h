
#ifndef BULLET_MANAGER_H
# define BULLET_MANAGER_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"

# define MAX_BULLETS 5000

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
};

class BulletManager : public CustomComponent
{
	private:
		Bullet bullets[MAX_BULLETS] = {0};
		int FindFreeIndex();
		bool bulletOutOfBound(t_Point pos);
	public:
		BulletManager();
		~BulletManager();
		void Start() override;
		void CreateBullet(t_Point pos, t_Point dir, float speed, t_Box color);
		void Update() override;
};

REGISTER_COMPONENT(BulletManager);

#endif

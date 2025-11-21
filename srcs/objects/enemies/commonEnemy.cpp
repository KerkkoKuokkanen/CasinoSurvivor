
#include "commonEnemy.h"

static bool HitBoxCheck(t_Box one, t_Box two)
{
	bool x = (one.x + one.w > two.x) && (one.x < two.x + two.w);
	bool y = (one.y + one.h > two.y) && (one.y > two.y + two.h);
	return (x && y);
}

int CommonEnemy::ApplyDamage(t_Point pos, float size, int damage, unsigned int name)
{
	float half = size * 0.5f;
	t_Box bullet = {pos.x - half, pos.y - half, size, size};
	if (!HitBoxCheck(bullet, hitbox))
		return (0);
	health -= damage;
	if (health <= 0)
		return (2);
	knownBullets.push_back(name);
	return (1);
}

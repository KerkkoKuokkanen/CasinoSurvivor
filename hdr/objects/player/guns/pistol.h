
#ifndef PISTOL_H
# define PISTOL_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"
# include "player.h"
# include "floorGrid.h"
# include "bulletManager.h"
# include "particles.h"
# include "crossHair.h"

class Pistol : public CustomComponent
{
	private:
		std::vector<t_Box> colors;
		uint64_t audioKey = 0;
		uint32_t damage = 4;
		float fireRate = 0.36f;
		float recharge = 0.0f;
		float recoil = 0.0f;
		Image *pistol = NULL;
		Image *hand1 = NULL;
		Image *hand2 = NULL;
		FloorGrid *grid = NULL;
		BulletManager *bullets = NULL;
		PlayerMovement *player = NULL;
		CrossHair *crossHair = NULL;
		Particles *parts = NULL;
		void PositionPistol();
		bool AnglePistol();
		void PistolAnglePosition();
		t_Point GetMousePoint();
		void GunBopping();
		void FullPosition();
		void Shooting();
		void ShootingAnimation();
		void CreateBullet();
		bool shooting = false;
		t_Point shootingForce = {0.0f, 0.0f};
		float cycle = 0.0f;
		t_Point shootDir = {1.0f, 0.0f};
	public:
		Pistol();
		~Pistol();
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(Pistol)

#endif

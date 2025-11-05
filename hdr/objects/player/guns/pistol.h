
#ifndef PISTOL_H
# define PISTOL_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"
# include "player.h"

class Pistol : public CustomComponent
{
	private:
		uint64_t audioKey = 0;
		uint32_t damage = 4;
		float fireRate = 0.33f;
		float recharge = 0.0f;
		Image *pistol = NULL;
		Image *hand1 = NULL;
		Image *hand2 = NULL;
		PlayerMovement *player = NULL;
		void PositionPistol();
		bool AnglePistol();
		void PistolAnglePosition();
		t_Point GetMousePoint();
		void GunBopping();
		void FullPosition();
		void Shooting();
		void ShootingAnimation();
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

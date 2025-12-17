
#ifndef PLAYER_H
# define PLAYER_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"
# include "floorGrid.h"
# include "camera.h"

class PlayerMovement : public CustomComponent
{
	private:
		uint64_t soundKey[4] = {0, 0, 0, 0};
		uint64_t jKey = 0;
		FloorGrid *grid = NULL;
		Camera *cam = NULL;
		Image *face = NULL;
		Image *glasses = NULL;
		Image *hair = NULL;
		Image *torso = NULL;
		Image *leftS = NULL;
		Image *rightS = NULL;
		t_Box hitBox = {0.0f, 0.0f, 0.0f, 0.0f};
		t_Point position = {0.0f, 0.0f};
		t_Point direction = {0.0f, 0.0f};
		float extraForce = 0.0f;
		float cyoteTime = 0.0f;
		float jumpBoost = 0.0f;
		int jumped = 0;
		bool heldDown = false;
		bool leftSliding = false;
		bool firstCycle = true;
		bool pressed = false;
		bool shoesInPosition = true;
		int leftRight = 0;
		int health = 0;
		float invisTime = 0.0f;
		float damageTime = 0.2f;
		bool cycle = false;
		bool PositionShoes();
		void ShoesWhilePressedF(float x);
		void ShoesWhilePressedB(float x);
		void MovePlayer();
		void AirMovement();
		void GetInputs();
		void HeadAnimation();
		void ShoesAnimation();
		void CameraMovement();
		void DamageAnimation();
		void Jump();
	public:
		bool inAir = false;
		bool canHit = true;
		t_Box GetHitBox() {return (hitBox);};
		PlayerMovement();
		~PlayerMovement();
		void ApplyXForce(float force);
		t_Point GetPosition() {return (position);};
		void Damage(int damage, float strength, t_Point pos);
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(PlayerMovement);

#endif


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
		FloorGrid *grid = NULL;
		Camera *cam = NULL;
		Image *face = NULL;
		Image *glasses = NULL;
		Image *hair = NULL;
		Image *torso = NULL;
		Image *leftS = NULL;
		Image *rightS = NULL;
		t_Point position = {0.0f, 0.0f};
		t_Point direction = {0.0f, 0.0f};
		float cyoteTime = 0.0f;
		float jumpBoost = 0.0f;
		int jumped = 0;
		bool heldDown = false;
		bool leftSliding = false;
		bool firstCycle = true;
		bool pressed = false;
		bool shoesInPosition = true;
		int leftRight = 0;
		bool PositionShoes();
		void ShoesWhilePressedF(float x);
		void ShoesWhilePressedB(float x);
		void MovePlayer();
		void AirMovement();
		void GetInputs();
		void HeadAnimation();
		void ShoesAnimation();
		void CameraMovement();
		void Jump();
	public:
		bool inAir = false;
		PlayerMovement();
		~PlayerMovement();
		t_Point GetPosition() {return (position);};
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(PlayerMovement);

#endif

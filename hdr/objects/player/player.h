
#ifndef PLAYER_H
# define PLAYER_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"

class PlayerMovement : public CustomComponent
{
	private:
		Image *face = NULL;
		Image *glasses = NULL;
		Image *hair = NULL;
		Image *torso = NULL;
		Image *leftH = NULL;
		Image *rightH = NULL;
		Image *leftS = NULL;
		Image *rightS = NULL;
		t_Point position = {0.0f, 0.0f};
		t_Point direction = {0.0f, 0.0f};
		bool leftSliding = false;
		bool firstCycle = true;
		bool pressed = false;
		bool inAir = false;
		bool shoesInPosition = true;
		int leftRight = 0;
		bool PositionShoes();
		void ShoesWhilePressedF(float x);
		void ShoesWhilePressedB(float x);
		void MovePlayer();
		void GetInputs();
		void HeadAnimation();
		void ShoesAnimation();
	public:
		PlayerMovement();
		~PlayerMovement();
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(PlayerMovement);

#endif

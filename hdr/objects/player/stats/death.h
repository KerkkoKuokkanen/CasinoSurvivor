
#ifndef DEATH_H
# define DEATH_H

# include "component.h"
# include "player.h"
# include "audioBars.h"
# include "particles.h"
# include "floorGrid.h"
# include "screenTransition.h"
# include "camShake.h"

class Death : public CustomComponent
{
	private:
		uint64_t soundKeys[3] = {0, 0, 0};
		PlayerMovement *player = NULL;
		AudioBars *audio = NULL;
		FloorGrid *grid = NULL;
		Particles *parts = NULL;
		ScreenTransition *trans = NULL;
		CameraShake *shake = NULL;
		Image *gameOver = NULL;
		float deathTime = -0.1f;
		float gameOverTime = -0.1f;
		float addTime = 0.0f;
		float multi = -1.0f;
		bool done = false;
		t_Point position = {0.0f, 0.0f};
		t_Point direction = {0.0f, 0.0f};
		std::vector<t_Box> colors = {};
		void DeathAnimation();
		void GameOverAnim();
	public:
		~Death();
		void Start();
		void Update();
};

REGISTER_COMPONENT(Death);

#endif

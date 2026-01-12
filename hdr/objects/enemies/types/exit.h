
#ifndef EXIT_ENEMY_H
# define EXIT_ENEMY_H

#include "commonEnemy.h"
#include "image.h"
#include "screenTransition.h"
#include "particles.h"
#include "audioBars.h"

class ExitEnemy : public CommonEnemy
{
	private:
		Image *img = NULL;
		Image *death = NULL;
		Particles *parts = NULL;
		ScreenTransition *screenTrans = NULL;
		AudioBars *musicVol = NULL;
		float cycle = 0.0f;
		float screenTransTime = 0.0f;
		bool particlesDone = false;
		void UpdateDeath();
	public:
		ExitEnemy();
		~ExitEnemy();
		void Update() override;
};

#endif

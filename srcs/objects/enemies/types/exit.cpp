
#include "exit.h"
#include "deltaTime.h"
#include "component.h"
#include "audio.h"

#define HIT_BOX_DIM 2.25f
#define HIT_BOX_HALF 1.125f
#define SCREEN_TRANSITION_TIME 0.22f

ExitEnemy::ExitEnemy()
{
	health = 4;
	independent = true;
	type = 0;
	deathForce = 0.725f;
	deathForceRadius = 1.35f;
	hitbox = {11.5f - HIT_BOX_HALF, 0.0f - HIT_BOX_HALF, HIT_BOX_DIM, HIT_BOX_DIM};
	img = new Image("exitSign", {11.5f, 0.0f, 3.0f, 3.0f}, 0.0f, 7);
	img->SetColor(0.9f, 0.9f, 0.9f, 1.0f);
	position = {13.0f, 0.0f};
	death = new Image("exitSignWhite", {11.5f, 0.0f, 3.0f, 3.0f}, 0.0f, 7);
	death->drawActive = false;
	death->SetColor(0.21f, 0.72f, 0.07f, 0.85f);
	death->drawDepth = -1.0f;
	screenTrans = new ScreenTransition();
	screenTrans->removeSelf = false;
	screenTrans->SetTransitionType(1);
	SystemObj *obj = FindSystemObject(10158550851708924347LU);
	parts = (Particles*)obj->GetComponent("Particles");
	obj = FindSystemObject(17206662188527305259LU);
	musicVol = (AudioBars*)obj->GetComponent("AudioBars");
}

ExitEnemy::~ExitEnemy()
{
	if (img != NULL)
		delete img;
	if (death != NULL)
		delete death;
	if (screenTrans != NULL)
		delete screenTrans;
}

void ExitEnemy::UpdateDeath()
{
	if (screenTrans->time <= SCREEN_TRANSITION_TIME)
	{
		RoomSwitch(1);
		return ;
	}
	if (!particlesDone)
	{
		PlaySound("glass", 9.0f, 0);
		musicVol->FadeMusic();
		death->position = img->position;
		death->drawActive = true;
		std::vector<t_Box> colors = {{0.21f, 0.72f, 0.07f, 1.0f},
									{0.21f * 0.5f, 0.72f * 0.5f, 0.07f * 0.5f, 1.0f},
									{0.21f * 0.75f, 0.72f * 0.75f, 0.07f * 0.75f, 1.0f},
									{0.21f * 1.25f, 0.72f * 1.25f, 0.07f * 1.25f, 1.0f}};
		particlesDone = true;
		parts->AddParticles(position, 2.0f, {1.0f, 0.0f}, 1.0f * PI, colors, 1.5f, 6.0f, 1000, "everyColor", 0.06f, 0.08f, 1.2f, 2.2f, 1.0f);
	}
	float a = DeltaTimeReal() * 2.0f;
	float b = 3.0f * a;
	death->dimentions = {death->dimentions.x * (1.0f + b), death->dimentions.y * (1.0f + b)};
	img->dimentions = {img->dimentions.x * (1.0f + 1.3f * b), img->dimentions.y * (1.0f - b)};
	t_Box col1 = img->GetColor();
	t_Box col2 = death->GetColor();
	img->SetColor(1.0f, 1.0f, 1.0f, col1.h - a);
	death->SetColor(0.21f, 0.72f, 0.07f, col2.h - a);
	if (screenTransTime >= 1.75f)
		screenTrans->Update();
	screenTransTime += DeltaTimeReal();
}

void ExitEnemy::Update()
{
	if (health <= 0)
	{
		UpdateDeath();
		return ;
	}
	float angle = sin(cycle);
	img->angle = angle * 0.33f;;
	img->position.y += angle * 0.01f;
	if (position.x > 4.5f)
	{
		position.x -= DeltaTime() * 1.5f;
		img->position.x = position.x;
	}
	position.y = img->position.y;
	hitbox = {position.x - HIT_BOX_HALF, position.y - HIT_BOX_HALF, HIT_BOX_DIM, HIT_BOX_DIM};
	cycle += DeltaTime() * 2.0f;
}

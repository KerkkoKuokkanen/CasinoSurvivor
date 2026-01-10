
#include "death.h"
#include "audio.h"
#include "mouse.h"

#define DEATH_TIME 0.6f
#define DEATH_ADD_TIME 0.1f
#define DEATH_Y -5.4f
#define DEATH_GAME_OVER 2.0f

void Death::Start()
{
	SystemObj *obj = FindSystemObject(2830363874434601420LU);
	player = (PlayerMovement*)obj->GetComponent("PlayerMovement");
	obj = FindSystemObject(17206662188527305259LU);
	audio = (AudioBars*)obj->GetComponent("AudioBars");
	grid = (FloorGrid*)obj->GetComponent("FloorGrid");
	obj = FindSystemObject(10158550851708924347LU);
	parts = (Particles*)obj->GetComponent("Particles");
	gameOver = new Image("gameOver", {0.0f, 0.0f, 10.0f, 5.0f}, 0.0f, 13);
	gameOver->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
	gameOver->SetTransformType(n_TransformTypes::TRANSFORM_STATIC);
	gameOver->drawDepth = 500.0f;
	gameOver->drawActive = false;
	trans = (ScreenTransition*)self->GetComponent("ScreenTransition");
	trans->active = false;
	obj = FindSystemObject(6302736476082374709LU);
	shake = (CameraShake*)obj->GetComponent("CameraShake");
}

void Death::DeathAnimation()
{
	if (deathTime <= 0.0f)
		return ;
	if (addTime >= DEATH_ADD_TIME)
	{
		parts->AddParticles({position.x, DEATH_Y - 0.5f}, 2.0f, direction, PI * 0.04f, colors, 1.0f, 22.0f, 333, "particle", 0.075f, 0.125f, 0.8f, 4.4f, 1.5f);
		grid->ApplyForce(position, 2.25f * multi, 3.0f);
		multi *= -1.0f;
		addTime -= DEATH_ADD_TIME;
	}
	deathTime -= DeltaTimeReal();
	addTime += DeltaTimeReal();
}

void Death::GameOverAnim()
{
	if (!done)
		return ;
	static float x = 0.0f;
	if (gameOverTime >= DEATH_GAME_OVER)
	{
		if (MouseKeyReleased(n_MouseKeys::MOUSE_LEFT))
			trans->active = true;
		gameOver->drawActive = true;
		gameOver->position = {gameOver->position.x, gameOver->position.y + sin(x) * 0.005f};
		x += DeltaTimeReal() * 4.0f;
		if (x >= PI * 2.0f)
			x = 0.0f;
		return ;
	}
	if (MouseKeyReleased(n_MouseKeys::MOUSE_LEFT))
		gameOverTime = 3.0f;
	gameOverTime += DeltaTimeReal();
}

void Death::Update()
{
	GameOverAnim();
	DeathAnimation();
	if (trans->time <= 0.26f)
		RoomSwitch(GetCurrentRoom(), {});
	if (player->GetPosition().y < -14.0f && !done)
	{
		float x = player->GetPosition().x;
		if (x < -13.0f)
			x = -13.0f;
		else if (x > 10.1f)
			x = 10.1f;
		position = {x, DEATH_Y};
		grid->ApplyForce(position, 2.25f, 3.0f);
		direction = VectorNormalize({-x, -DEATH_Y * 2.0f});
		deathTime = DEATH_TIME;
		addTime = DEATH_ADD_TIME;
		player->self->active = false;
		audio->active = false;
		audio->StopMusic();
		done = true;
		soundKeys[0] = RePlaySound("death7", 82.0f, 0, soundKeys[0]);
		soundKeys[1] = RePlaySound("death3", 8.0f, 0, soundKeys[1]);
		soundKeys[2] = RePlaySound("death4", 1.0f, 0, soundKeys[2]);
		t_Box col = grid->GetColorMatchingPoint(x);
		colors.push_back({col.x, col.y, col.w, 0.9f});
		col = grid->GetColorMatchingPoint(x + 2.0f);
		colors.push_back({col.x, col.y, col.w, 0.9f});
		col = grid->GetColorMatchingPoint(x + 4.0f);
		colors.push_back({col.x, col.y, col.w, 0.9f});
		col = grid->GetColorMatchingPoint(x + 6.0f);
		colors.push_back({col.x, col.y, col.w, 0.9f});
		col = grid->GetColorMatchingPoint(x + 8.0f);
		colors.push_back({col.x, col.y, col.w, 0.9f});
		shake->CreateCameraShake(0.08f);
	}
}

Death::~Death()
{
	if (gameOver != NULL)
		delete gameOver;
}

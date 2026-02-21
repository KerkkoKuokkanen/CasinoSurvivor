
#include "playButton.h"
#include "audio.h"

#define RESERVED_CHANNEL_ONE 61
#define RESERVED_CHANNEL_TWO 62

static float vol1 = 40.0f;
static float vol2 = 16.0f;

static void PlayMusic()
{
	ReserveChannel(RESERVED_CHANNEL_ONE);
	ReserveChannel(RESERVED_CHANNEL_TWO);
	vol1 = 40.0f;
	vol2 = 16.0f;
	Mix_Chunk *chunk1 = GetSoundWithName("casinoTestDrum2");
	Mix_Chunk *chunk2 = GetSoundWithName("casinoTestNoDrum");
	Mix_Volume(RESERVED_CHANNEL_ONE, (int)vol1);
	Mix_Volume(RESERVED_CHANNEL_TWO, (int)vol2);
	Mix_PlayChannel(RESERVED_CHANNEL_ONE, chunk1, -1);
	Mix_PlayChannel(RESERVED_CHANNEL_TWO, chunk2, -1);
}

static void FadeMusic(float stepNum)
{
	static const float vol1Scale = 40.0f / 20.0f;
	static const float vol2Scale = 16.0f / 20.0f;
	vol1 = vol1Scale * stepNum;
	vol2 = vol2Scale * stepNum;
	Mix_Volume(RESERVED_CHANNEL_ONE, (int)vol1);
	Mix_Volume(RESERVED_CHANNEL_TWO, (int)vol2);
}


PlayButton::PlayButton()
{
	playTextFront = new Image("playText", {7.61f, -3.1f, 2.5f, 0.9f}, 0.0f, 7);
	playTextBack = new Image("playText", {7.71f, -3.2f, 2.5f, 0.9f}, 0.0f, 7);
	front = new Image("everyColor2", {7.55f, -3.15f, 2.6f, 1.6f}, 0.0f, 7);
	back = new Image("everyColor2", {7.55f, -3.15f, 2.95f, 1.85f}, 0.0f, 7);
	front->SetColor(0.0f, 0.0f, 0.0f, 0.8f);
	front->drawDepth = 3.0f;
	back->SetColor(1.0f, 1.0f, 1.0f, 0.6f);
	back->drawDepth = 2.0f;
	playTextFront->drawDepth = 5.0f;
	playTextBack->drawDepth = 4.0f;
	playTextFront->SetColor(181.0f / 255.0f, 100.0f / 255.0f, 18.0f / 255.0f, 1.0f);
	playTextBack->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	button = new Button();

	button->SetButtonState(0, back, {1.0f, 1.0f, 1.0f, 0.6f});
	button->SetButtonState(1, back, {252.0f / 300.0f, 186.0f / 300.0f, 40.0f / 300.0f, 0.8f});
	button->SetButtonState(2, back, {252.0f / 460.0f, 186.0f / 460.0f, 40.0f / 460.0f, 0.8f});
	button->SetButtonState(3, back, {1.0f, 1.0f, 1.0f, 0.6f});
	button->SetButtonFunction([=]() { SetPlay(); });

	screenTrans = new ScreenTransition();
	screenTrans->SetTransitionType(1);
	screenTrans->active = false;
	screenTrans->removeSelf = false;
}

PlayButton::~PlayButton()
{
	delete playTextFront;
	delete playTextBack;
	delete front;
	delete back;
	delete screenTrans;
}

void PlayButton::Start()
{
	PlayMusic();
}

void PlayButton::SetPlay()
{
	play = true;
}

void PlayButton::Update()
{
	if (play)
	{
		if (screenTrans->time <= 0.22f)
		{
			UnReserveChannel(RESERVED_CHANNEL_ONE);
			UnReserveChannel(RESERVED_CHANNEL_TWO);
			RoomSwitch(3);
			return ;
		}
		screenTrans->Update();
		while (fadeTimer >= 0.0375)
		{
			fadeIndex = fmax(0.0f, fadeIndex - 1.0f);
			fadeTimer -= 0.0375f;
		}
		FadeMusic(fadeIndex);
		fadeTimer += DeltaTimeReal();
		return ;
	}
	button->Update();
	if (button->clickState == 1)
	{
		playTextFront->dimentions = {2.6f, 0.95f};
		playTextBack->dimentions = {2.6f, 0.95f};
	}
	else
	{
		playTextFront->dimentions = {2.5f, 0.9f};
		playTextBack->dimentions = {2.5f, 0.9f};
	}
}

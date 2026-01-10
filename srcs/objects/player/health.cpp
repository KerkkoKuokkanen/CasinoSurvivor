
#include "health.h"
#include "gameStats.h"
#include "audio.h"

Health::Health()
{
	ResetStats();	//Temporarily here
	pill = new Image("pill2", {9.32f, 5.0f, 1.1f, 1.1f}, 0.0f, 13);
	pill->SetTransformType(n_TransformTypes::TRANSFORM_STATIC);
	pill->drawDepth = 200.0f;
	num = new Number({8.28f, 5.0f}, std::to_string(health), 0.7f, {0.8f, 0.8f, 0.8f, 1.0f}, 200000.0f);
	numPos = {8.28, 5.0f};
	health = GetHealth();
}

Health::~Health()
{
	if (pill)
		delete pill;
	if (num)
		delete num;
}

void Health::Damage(int damage)
{
	int dmg = damage;
	if (dmg < 4)
		dmg = 4;
	else if (dmg > 30)
		dmg = 30;
	float r = 0.8f;
	float g = 0.0f;
	float b = 0.0f;
	float sm = (0.1f / 26.0f) * ((float)dmg - 4.0f);
	numberShakeMag = sm + 0.1f;
	numShakeColor = {r, g, b, 1.0f};
	
	numAddTime = 0.6f / ((float)damage);
	numShakeTime = 0.38f;
	numShakeMini = 0.07f;
	health = GetHealth();
	cycle2 = true;

	float souncScale = 0.6f / 26.0f;
	float soundMulti = 1.0f + (souncScale * (float)(dmg - 4));
	soundKey[0] = RePlaySound("hit1", 20.0f * soundMulti, 0, soundKey[0]);
	soundKey[1] = RePlaySound("hit2", 4.0f * soundMulti, 0, soundKey[1]);
	soundKey[2] = RePlaySound("hit3", 24.0f * soundMulti, 0, soundKey[2]);
	soundKey[3] = RePlaySound("death2", 24.0f * soundMulti, 0, soundKey[3]);
}

void Health::Hfunc1()
{
	if (numShakeTime <= 0.0f)
	{
		num->SetPosition(numPos);
		num->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
		return ;
	}
	if (numShakeMini >= 0.06f)
	{
		numShakeMini = 0.0f;
		float ySign = (rand() % 2 == 0) ? 1.0f : -1.0f;
		float xSign = (rand() % 2 == 0) ? 1.0f : -1.0f;
		num->SetPosition({numPos.x + numberShakeMag * xSign, numPos.y + numberShakeMag * ySign});
		if (cycle2)
		{
			num->SetColor(numShakeColor.x, numShakeColor.y, numShakeColor.w, 1.0f);
			cycle2 = false;
		}
		else
		{
			num->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
			cycle2 = true;
		}
	}
	numShakeMini += DeltaTimeReal();
	numShakeTime -= DeltaTimeReal();
}

void Health::Hfunc2()
{
	if (num->num == health)
		return ;
	if (numberAddTime >= numAddTime)
	{
		int amount = rounding(numberAddTime / numAddTime);
		int n = num->num + amount;
		if (n > health)
			n = health;
		delete num;
		std::string nn = std::to_string(n);
		float posX = 8.28f - (nn.length() - 1) * 0.7f;
		num = new Number({posX, 5.0f}, nn, 0.7f, {0.8f, 0.8f, 0.8f, 1.0f}, 220.0f);
		numPos = {posX, 5.0f};
		numberAddTime = 0.0f;
	}
	numberAddTime += DeltaTimeReal();
}

void Health::Update()
{
	Hfunc1();
	Hfunc2();
}

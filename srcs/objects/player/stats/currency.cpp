
#include "currency.h"
#include "gameStats.h"

#define CHIP_MAX_CHANGE_TIME 0.2f

Currency::Currency()
{
	chipAmount = GetMoney();
	chips = new Image("AllChips", {9.26f, -5.0f, 1.25f, 1.25f}, 0.0f, 13);
	chips->SetTransformType(n_TransformTypes::TRANSFORM_STATIC);
	chips->drawDepth = 200.0f;
	chips->SetColor(0.85f, 0.85f, 0.85f, 1.0f);
	chipNumber = new Number({8.28f, -5.0f}, std::to_string(chipAmount), 0.7f, {0.75f, 0.75f, 0.75f, 1.0f}, 200000.0f);
	std::string nn = std::to_string(chipAmount);
	float posX = 8.28f - (nn.length() - 1) * 0.7f;
	chipNumber->SetPosition({posX, -5.0f});
}

Currency::~Currency()
{
	delete chips;
	delete chipNumber;
}

void Currency::GetAddNum()
{
	int money = GetMoney();
	if (chipAmount == money)
		return ;
	int needToAdd = money - chipNumber->num;
	numAddTime = CHIP_MAX_CHANGE_TIME / ((float)needToAdd);
	chipAmount = money;
}

void Currency::Update()
{
	GetAddNum();
	if (chipNumber->num == chipAmount)
		return ;
	if (numAddTimer >= fabs(numAddTime))
	{
		int amount = rounding(numAddTimer / numAddTime);
		int n = chipNumber->num + amount;
		if (amount < 0 && chipAmount - amount < 0)
			n = 0;
		if (n > chipAmount && amount >= 0)
			n = chipAmount;
		delete chipNumber;
		std::string nn = std::to_string(n);
		float posX = 8.28f - (nn.length() - 1) * 0.7f;
		chipNumber = new Number({posX, -5.0f}, nn, 0.7f, {0.75f, 0.75f, 0.75f, 1.0f}, 220.0f);
		numAddTimer = 0.0f;
	}
	numAddTimer += DeltaTimeReal();
}


#include "BjBets.h"
#include "mouse.h"
#include "mouseOver.h"
#include "chipMove.h"
#include "gameStats.h"

static int GetChipAmount(int chosen)
{
	switch (chosen)
	{
		case 0:
			return 1;
		case 1:
			return 5;
		case 2:
			return 10;
		case 3:
			return 50;
		case 4:
			return 100;
		case 5:
			return 500;
	}
	return 1;
}

BjBets::BjBets()
{
	bet = new Image("bet", {0.0f, 1.22f, 20.0f, 12.71f}, 0.0f, 2);
	pp = new Image("pp", {0.0f, 1.22f, 20.0f, 12.71f}, 0.0f, 2);
	combo = new Image("21+3", {0.0f, 1.22f, 20.0f, 12.71f}, 0.0f, 2);
	hovered = new Image("bet", {0.0f, 1.97f, 23.40f, 14.86f}, 0.0f, 2);
	hovered->drawDepth = -1.0f;
	hovered->SetColor(1.0f, 0.85f, 0.0f, 1.0f);
	hovered->drawActive = false;
}

void BjBets::DeleteNums()
{
	if (numBet != NULL)
		delete numBet;
	if (numPP != NULL)
		delete numPP;
	if (numCombo != NULL)
		delete numCombo;
	numBet = NULL;
	numPP = NULL;
	numCombo = NULL;
}

BjBets::~BjBets()
{
	delete bet;
	delete pp;
	delete combo;
	delete hovered;
	DeleteNums();
}

void BjBets::ResetBetAmounts()
{
	bzero(betAmounts, sizeof(int) * 3);
}

void BjBets::ManageNumber()
{
	if (betAmounts[0] == 0 && betAmounts[1] == 0 && betAmounts[2] == 0)
	{
		DeleteNums();
		return ;
	}
	if (betAmounts[0] != 0)
	{
		if (numBet == NULL)
			numBet = new Number({0.0f, -4.15f}, std::to_string(betAmounts[0]), 0.33f, {0.83f, 0.83f, 0.83f, 1.0f}, 1.0f);
		else if (numBet->num != betAmounts[0])
		{
			delete numBet;
			numBet = new Number({0.0f, -4.15f}, std::to_string(betAmounts[0]), 0.33f, {0.83f, 0.83f, 0.83f, 1.0f}, 1.0f);
		}
		int l = std::to_string(betAmounts[0]).length() - 1;
		float start = (l > 1) ? -0.03f : 0.0f;
		numBet->SetPosition({start - 0.33f * ((float)l * 0.5f), -4.15f});
	}
	if (betAmounts[1] != 0)
	{
		if (numPP == NULL)
			numPP = new Number({1.77f, -3.9f}, std::to_string(betAmounts[1]), 0.33f, {0.83f, 0.83f, 0.83f, 1.0f}, 1.0f);
		else if (numPP->num != betAmounts[1])
		{
			delete numPP;
			numPP = new Number({1.77f, -3.9f}, std::to_string(betAmounts[1]), 0.33f, {0.83f, 0.83f, 0.83f, 1.0f}, 1.0f);
		}
		int l = std::to_string(betAmounts[1]).length() - 1;
		float start = (l > 1) ? 1.77f - 0.03f : 1.77f;
		numPP->SetPosition({start - 0.33f * ((float)l * 0.5f), -3.9f});
	}
	if (betAmounts[2] != 0)
	{
		if (numCombo == NULL)
			numCombo = new Number({-1.76f, -3.9f}, std::to_string(betAmounts[2]), 0.33f, {0.83f, 0.83f, 0.83f, 1.0f}, 1.0f);
		else if (numCombo->num != betAmounts[2])
		{
			delete numCombo;
			numCombo = new Number({-1.76f, -3.9f}, std::to_string(betAmounts[2]), 0.33f, {0.83f, 0.83f, 0.83f, 1.0f}, 1.0f);
		}
		int l = std::to_string(betAmounts[2]).length() - 1;
		float start = (l > 1) ? -1.76f - 0.03f : -1.76f;
		numCombo->SetPosition({start - 0.33f * ((float)l * 0.5f), -3.9f});
	}
}

void BjBets::Update()
{
	hovered->drawActive = false;
	ManageNumber();
	if (chosen == -1)
		return ;
	hovered->SetColor(1.0f, 0.85f, 0.0f, 1.0f);
	int visited = -1;
	if (MouseOverImage(bet))
	{
		visited = 0;
		hovered->drawActive = true;
		hovered->position = {0.0f, 1.97f};
	}
	else if (MouseOverImage(pp))
	{
		visited = 1;
		hovered->drawActive = true;
		hovered->position = {1.765f, 2.075f};
	}
	else if (MouseOverImage(combo))
	{
		visited = 2;
		hovered->drawActive = true;
		hovered->position = {-1.74f, 2.08f};
	}
	if (MouseKeyPressed(n_MouseKeys::MOUSE_LEFT) && visited >= 0)
	{
		int money = GetMoney();
		SetMoney(money - GetChipAmount(chosen));
		betAmounts[visited] += GetChipAmount(chosen);
		ChipMove *m = (ChipMove*)self->AddComponent("ChipMove");
		m->SetUp(chosen, visited);
	}
	if (visited >= 0 && MouseKeyHeld(n_MouseKeys::MOUSE_LEFT))
		hovered->SetColor(0.5f, 0.425f, 0.0f, 1.0f);
}

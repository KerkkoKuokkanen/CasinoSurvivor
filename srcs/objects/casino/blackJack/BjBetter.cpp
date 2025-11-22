
#include "bjBetter.h"
#include "stats.h"
#include "mouse.h"
#include "mouseOver.h"

BjBetter::BjBetter()
{
	chip1 = new Image("chip1", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 2);
	chip5 = new Image("chip5", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 2);
	chip10 = new Image("chip10", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 2);
	chip50 = new Image("chip50", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 2);
	chip100 = new Image("chip100", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 2);
	chip500 = new Image("chip500", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 2);
	bet = new Image("BJBet", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 2);
	clear = new Image("BJClear", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 2);
	chip1->position = {-2.0f, -9.0f};
	chip5->position = {-1.2f, -9.0f};
	chip10->position = {-0.4f, -9.0f};
	chip50->position = {0.4f, -9.0f};
	chip100->position = {1.2f, -9.0f};
	chip500->position = {2.0f, -9.0f};
	bet->position = {2.9f, -9.1f};
	clear->position = {-2.9f, -9.1f};
	buttons[0].SetButtonState(0, chip1, {0.6f, 0.6f, 0.6f, 1.0f});
	buttons[0].SetButtonState(3, chip1, {0.1f, 0.1f, 0.1f, 1.0f});
	buttons[0].SetButtonState(1, chip1, {0.9f, 0.9f, 0.9f, 1.0f});
	buttons[0].SetButtonState(2, chip1, {0.75, 0.75f, 0.75f, 1.0f});
	buttons[1].SetButtonState(0, chip5, {0.6f, 0.6f, 0.6f, 1.0f});
	buttons[1].SetButtonState(3, chip5, {0.1f, 0.1f, 0.1f, 1.0f});
	buttons[1].SetButtonState(1, chip5, {0.9f, 0.9f, 0.9f, 1.0f});
	buttons[1].SetButtonState(2, chip5, {0.75f, 0.75f, 0.75f, 1.0f});
	buttons[2].SetButtonState(0, chip10, {0.6f, 0.6f, 0.6f, 1.0f});
	buttons[2].SetButtonState(3, chip10, {0.1f, 0.1f, 0.1f, 1.0f});
	buttons[2].SetButtonState(1, chip10, {0.9f, 0.9f, 0.9f, 1.0f});
	buttons[2].SetButtonState(2, chip10, {0.75f, 0.75f, 0.75f, 1.0f});
	buttons[3].SetButtonState(0, chip50, {0.6f, 0.6f, 0.6f, 1.0f});
	buttons[3].SetButtonState(3, chip50, {0.1f, 0.1f, 0.1f, 1.0f});
	buttons[3].SetButtonState(1, chip50, {0.9f, 0.9f, 0.9f, 1.0f});
	buttons[3].SetButtonState(2, chip50, {0.75f, 0.75f, 0.75f, 1.0f});
	buttons[4].SetButtonState(0, chip100, {0.6f, 0.6f, 0.6f, 1.0f});
	buttons[4].SetButtonState(3, chip100, {0.1f, 0.1f, 0.1f, 1.0f});
	buttons[4].SetButtonState(1, chip100, {0.9f, 0.9f, 0.9f, 1.0f});
	buttons[4].SetButtonState(2, chip100, {0.75f, 0.75f, 0.75f, 1.0f});
	buttons[5].SetButtonState(0, chip500, {0.6f, 0.6f, 0.6f, 1.0f});
	buttons[5].SetButtonState(3, chip500, {0.1f, 0.1f, 0.1f, 1.0f});
	buttons[5].SetButtonState(1, chip500, {0.9f, 0.9f, 0.9f, 1.0f});
	buttons[5].SetButtonState(2, chip500, {0.75f, 0.75f, 0.75f, 1.0f});
	buttons[6].SetButtonState(0, bet, {0.6f, 0.6f, 0.6f, 1.0f});
	buttons[6].SetButtonState(3, bet, {0.2f, 0.2f, 0.2f, 1.0f});
	buttons[6].SetButtonState(1, bet, {0.9f, 0.9f, 0.9f, 1.0f});
	buttons[6].SetButtonState(2, bet, {0.75f, 0.75f, 0.75f, 1.0f});
	buttons[7].SetButtonState(0, clear, {0.6f, 0.6f, 0.6f, 1.0f});
	buttons[7].SetButtonState(3, clear, {0.2f, 0.2f, 0.2f, 1.0f});
	buttons[7].SetButtonState(1, clear, {0.9f, 0.9f, 0.9f, 1.0f});
	buttons[7].SetButtonState(2, clear, {0.75f, 0.75f, 0.75f, 1.0f});
}

BjBetter::~BjBetter()
{
	delete chip1;
	delete chip5;
	delete chip10;
	delete chip50;
	delete chip100;
	delete chip500;
	delete bet;
	delete clear;
}

void BjBetter::ManageChips()
{
	uint64_t chips = GetPlayerChips();
	buttons[0].notUsable = false;
	buttons[1].notUsable = false;
	buttons[2].notUsable = false;
	buttons[3].notUsable = false;
	buttons[4].notUsable = false;
	buttons[5].notUsable = false;
	buttons[6].notUsable = false;
	buttons[7].notUsable = false;
	if (chips < 1)
		buttons[0].notUsable = true;
	if (chips < 5)
		buttons[1].notUsable = true;
	if (chips < 10)
		buttons[2].notUsable = true;
	if (chips < 50)
		buttons[3].notUsable = true;
	if (chips < 100)
		buttons[4].notUsable = true;
	if (chips < 500)
		buttons[5].notUsable = true;
	if (chipsBet == 0)
	{
		buttons[6].notUsable = true;
		buttons[7].notUsable = true;
	}
}

void BjBetter::ManageButtons()
{
	buttons[0].Update();
	buttons[1].Update();
	buttons[2].Update();
	buttons[3].Update();
	buttons[4].Update();
	buttons[5].Update();
	buttons[6].Update();
	buttons[7].Update();
}

void BjBetter::Update()
{
	if (!active)
		return ;
	ManageChips();
	ManageButtons();
}

void BjBetter::Activate()
{
	active = true;
	chip1->drawActive = true;
	chip5->drawActive = true;
	chip10->drawActive = true;
	chip50->drawActive = true;
	chip100->drawActive = true;
	chip500->drawActive = true;
	bet->drawActive = true;
	clear->drawActive = true;
}

void BjBetter::Deactivate()
{
	active = false;
	chip1->drawActive = false;
	chip5->drawActive = false;
	chip10->drawActive = false;
	chip50->drawActive = false;
	chip100->drawActive = false;
	chip500->drawActive = false;
	bet->drawActive = false;
	clear->drawActive = false;
}

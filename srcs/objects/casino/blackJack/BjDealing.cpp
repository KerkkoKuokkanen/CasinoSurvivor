
#include "BjDealing.h"
#include "keyboard.h"
#include "mouse.h"
#include "winIndicator.h"

void BjDealing::ResetDealing()
{
	usedCards.clear();
	currentDeck->drawActive = false;
	buttons[0]->drawActive = false;
	buttons[1]->drawActive = false;
	buttons[2]->drawActive = false;
	buttons[3]->drawActive = false;
	currentDeal = {false, false, false, {0.0f, 0.0f}, NULL, -1, -1, -1.0f};
	dealQueue.push({true, true, false, {-0.07f, -1.63f}, NULL, -1, 1, 0.42f});
	dealQueue.push({false, true, false, {-0.07f, 2.95f}, NULL, -1, 0, 0.42f});
	dealQueue.push({true, true, false, {0.34f, -1.33f}, NULL, -1, 1, 0.42f});
	dealQueue.push({false, false, false, {0.34f, 3.25f}, NULL, -1, 0, 0.42f});
	if (deck->needToSuffle)
		deck->SuffleCards();
	bet->roundActive = false;
	order = -10.0f;
	currDeck = 0;
	inVoid = false;
}

BjDealing::~BjDealing()
{
	if (currentDeck != NULL)
		delete currentDeck;
	if (buttons[0] != NULL)
		delete buttons[0];
	if (buttons[1] != NULL)
		delete buttons[1];
	if (buttons[2] != NULL)
		delete buttons[2];
	if (buttons[3] != NULL)
		delete buttons[3];
}

void BjDealing::Start()
{
	currentDeck = new Image("cardBlank", {-0.07f, -1.63f, 1.55f, 1.95f}, 0.0f, 5);
	currentDeck->drawActive = false;
	currentDeck->drawDepth = -11.0f;
	currentDeck->SetColor(0.788f, 0.627f, 0.03f, 1.0f);
	buttons[0] = new Image("BJSplit", {-2.25f, -4.9f, 1.5f, 1.5f}, 0.0f, 7);
	buttons[1] = new Image("BJStand", {-0.75f, -4.9f, 1.5f, 1.5f}, 0.0f, 7);
	buttons[2] = new Image("BJHit", {0.75f, -4.9f, 1.5f, 1.5f}, 0.0f, 7);
	buttons[3] = new Image("BJ2X", {2.25f, -4.9f, 1.5f, 1.5f}, 0.0f, 7);
	buttons[0]->drawActive = false;
	buttons[1]->drawActive = false;
	buttons[2]->drawActive = false;
	buttons[3]->drawActive = false;
	buttons[0]->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
	buttons[1]->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
	buttons[2]->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
	buttons[3]->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
	deck = (BjDeck*)self->GetComponent("BjDeck");
	bet = (BjBetter*)self->GetComponent("BjBetter");
	ResetDealing();
}

void BjDealing::GetDealtCard()
{
	if (currentDeal.time > 0.0f)
		return ;
	if (dealQueue.size() <= 0)
		return ;
	if (currentDeal.index != -1)
		usedCards.push_back(currentDeal);
	currentDeal = dealQueue.front();
	currentDeal.card = deck->TakeCard(&currentDeal.index);
	deck->PositionCards(currentDeal.pos, currentDeal.index);
	if (currentDeal.turn)
		deck->TurnCard(currentDeal.index);
	dealQueue.pop();
}

void BjDealing::UpdateDealtCard()
{
	if (currentDeal.time <= 0.0f)
		return ;
	if (currentDeal.time < 0.3f && currentDeal.ordered == false)
	{
		currentDeal.card->image->drawDepth = order;
		currentDeal.ordered = true;
		order += 0.1f;
	}
	currentDeal.time -= DeltaTime();
}

void BjDealing::Discard()
{
	for (int i = 0; i < usedCards.size(); i++)
		deck->PositionCards({3.5f, usedCards[i].pos.y + 10.0f}, usedCards[i].index);
	deck->PositionCards({3.5f, currentDeal.pos.y + 10.0f}, currentDeal.index);
}

void BjDealing::CheckButtons()
{

}

void BjDealing::CheckWins()
{
	if (inVoid)
		return ;
	std::vector<DealQueue> s1 = GetStackCards(1, usedCards, currentDeal);
	if (s1[0].)
}

void BjDealing::HandlePlaying()
{
	if (currentDeal.time > 0.0f || dealQueue.size() != 0)
	{
		buttons[0]->drawActive = false;
		buttons[1]->drawActive = false;
		buttons[2]->drawActive = false;
		buttons[3]->drawActive = false;
		currentDeck->drawActive = false;
		return ;
	}
	if (inVoid && MouseKeyPressed(n_MouseKeys::MOUSE_LEFT))
	{
		Discard();
		ResetDealing();
		bet->Activate(30);
		return ;
	}
	CheckWins();
	if (inVoid)
		return ;
	currentDeck->drawActive = true;
	buttons[0]->drawActive = true;
	buttons[1]->drawActive = true;
	buttons[2]->drawActive = true;
	buttons[3]->drawActive = true;
	CheckButtons();
}

void BjDealing::Update()
{
	if (deck->suffling || !bet->roundActive)
		return ;
	GetDealtCard();
	UpdateDealtCard();
	HandlePlaying();
}

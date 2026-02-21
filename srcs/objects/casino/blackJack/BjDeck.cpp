
#include "BjDeck.h"

#define X_POS 6.1f
#define Y_POS 4.32f

void BjDeck::SuffleValues()
{
	std::vector<std::tuple<int,int>> values;
	int suit = 0;
	for (int i = 0; i < 312; i++)
	{
		if (i % 13 == 0 && i != 0)
			suit = (suit == 3) ? 0 : suit + 1;
		int val = i % 13 + 1;
		values.push_back({val, suit});
	}
	for (int i = 0; i < DECK_SIZE; i++)
	{
		if (cards[i].value == (-1))
			continue ;
		int r = rand() % values.size();
		int num = std::get<0>(values[r]);
		int s = std::get<1>(values[r]);
		int val = (num >= 10) ? 10 : num;
		cards[i].number = num;
		cards[i].suit = s;
		cards[i].value = val;
		values.erase(values.begin() + r);
	}
}

BjDeck::BjDeck()
{
	int blank = rand() % 20 + (DECK_SIZE * 0.58f);
	float xScale = 2.8f / DECK_SIZE;
	for (int i = 0; i < DECK_SIZE; i++)
	{
		if (i == blank)
		{
			cards[i].image = new Image("cardBlank", {1.0f, 1.0f, 1.45f, 1.85f}, 0.0f, 5);
			cards[i].image->position = {X_POS + xScale * i + 0.1f, Y_POS - 0.08f};
			cards[i].image->SetColor(0.788f * 0.8, 0.627f * 0.8, 0.03f * 0.8, 1.0f);
			cards[i].image->drawDepth = -(float)i;
			cards[i].value = -1;
			cards[i].suit = -1;
			cards[i].number = -1;
			continue ;
		}
		cards[i].image = new Image("cardBack", {1.0f, 1.0f, 1.45f, 1.85f}, 0.0f, 5);
		cards[i].image->position = {X_POS + xScale * i, Y_POS};
		cards[i].image->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
		cards[i].image->drawDepth = -(float)i;
	}
	SuffleValues();
}

BjDeck::~BjDeck()
{
	for (int i = 0; i < DECK_SIZE; i++)
		delete cards[i].image;
}

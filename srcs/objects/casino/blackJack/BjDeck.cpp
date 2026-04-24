
#include "BjDeck.h"

#define X_POS 6.1f
#define Y_POS 4.32f

#define TURN_TIME 0.3f
#define MOVE_SPEED 18.0f

static uint64_t cardTexts[4][13];

static void AssignCardTexts()
{
	cardTexts[0][0] = GetTextureGLData("cardClubsA").hash;
	cardTexts[0][1] = GetTextureGLData("cardClubs2").hash;
	cardTexts[0][2] = GetTextureGLData("cardClubs3").hash;
	cardTexts[0][3] = GetTextureGLData("cardClubs4").hash;
	cardTexts[0][4] = GetTextureGLData("cardClubs5").hash;
	cardTexts[0][5] = GetTextureGLData("cardClubs6").hash;
	cardTexts[0][6] = GetTextureGLData("cardClubs7").hash;
	cardTexts[0][7] = GetTextureGLData("cardClubs8").hash;
	cardTexts[0][8] = GetTextureGLData("cardClubs9").hash;
	cardTexts[0][9] = GetTextureGLData("cardClubs10").hash;
	cardTexts[0][10] = GetTextureGLData("cardClubsJ").hash;
	cardTexts[0][11] = GetTextureGLData("cardClubsQ").hash;
	cardTexts[0][12] = GetTextureGLData("cardClubsK").hash;

	cardTexts[1][0] = GetTextureGLData("cardDiamondsA").hash;
	cardTexts[1][1] = GetTextureGLData("cardDiamonds2").hash;
	cardTexts[1][2] = GetTextureGLData("cardDiamonds3").hash;
	cardTexts[1][3] = GetTextureGLData("cardDiamonds4").hash;
	cardTexts[1][4] = GetTextureGLData("cardDiamonds5").hash;
	cardTexts[1][5] = GetTextureGLData("cardDiamonds6").hash;
	cardTexts[1][6] = GetTextureGLData("cardDiamonds7").hash;
	cardTexts[1][7] = GetTextureGLData("cardDiamonds8").hash;
	cardTexts[1][8] = GetTextureGLData("cardDiamonds9").hash;
	cardTexts[1][9] = GetTextureGLData("cardDiamonds10").hash;
	cardTexts[1][10] = GetTextureGLData("cardDiamondsJ").hash;
	cardTexts[1][11] = GetTextureGLData("cardDiamondsQ").hash;
	cardTexts[1][12] = GetTextureGLData("cardDiamondsK").hash;

	cardTexts[2][0] = GetTextureGLData("cardHeartsA").hash;
	cardTexts[2][1] = GetTextureGLData("cardHearts2").hash;
	cardTexts[2][2] = GetTextureGLData("cardHearts3").hash;
	cardTexts[2][3] = GetTextureGLData("cardHearts4").hash;
	cardTexts[2][4] = GetTextureGLData("cardHearts5").hash;
	cardTexts[2][5] = GetTextureGLData("cardHearts6").hash;
	cardTexts[2][6] = GetTextureGLData("cardHearts7").hash;
	cardTexts[2][7] = GetTextureGLData("cardHearts8").hash;
	cardTexts[2][8] = GetTextureGLData("cardHearts9").hash;
	cardTexts[2][9] = GetTextureGLData("cardHearts10").hash;
	cardTexts[2][10] = GetTextureGLData("cardHeartsJ").hash;
	cardTexts[2][11] = GetTextureGLData("cardHeartsQ").hash;
	cardTexts[2][12] = GetTextureGLData("cardHeartsK").hash;

	cardTexts[3][0] = GetTextureGLData("cardSpadesA").hash;
	cardTexts[3][1] = GetTextureGLData("cardSpades2").hash;
	cardTexts[3][2] = GetTextureGLData("cardSpades3").hash;
	cardTexts[3][3] = GetTextureGLData("cardSpades4").hash;
	cardTexts[3][4] = GetTextureGLData("cardSpades5").hash;
	cardTexts[3][5] = GetTextureGLData("cardSpades6").hash;
	cardTexts[3][6] = GetTextureGLData("cardSpades7").hash;
	cardTexts[3][7] = GetTextureGLData("cardSpades8").hash;
	cardTexts[3][8] = GetTextureGLData("cardSpades9").hash;
	cardTexts[3][9] = GetTextureGLData("cardSpades10").hash;
	cardTexts[3][10] = GetTextureGLData("cardSpadesJ").hash;
	cardTexts[3][11] = GetTextureGLData("cardSpadesQ").hash;
	cardTexts[3][12] = GetTextureGLData("cardSpadesK").hash;
}

void BjDeck::SuffleValues()
{
	std::vector<std::tuple<int,int>> values;
	int suit = 0;
	for (int i = 0; i < 312; i++)
	{
		if (i % 13 == 0 && i != 0)
			suit = (suit == 3) ? 0 : suit + 1;
		int val = i % 13;
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
	AssignCardTexts();
	bzero(cards, sizeof(Cards) * DECK_SIZE);
	int blank = rand() % 20 + (DECK_SIZE * 0.58f);
	float xScale = 2.8f / DECK_SIZE;
	for (int i = 0; i < DECK_SIZE; i++)
	{
		cards[i].faceUp = true;
		cards[i].movePos = {-31.0f, -31.0f};
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

void BjDeck::TurnCard(int index)
{
	cards[index].turnTime = TURN_TIME;
}

static void FlattenCard(Image *img, float time)
{
	float scale = 1.45f / (TURN_TIME * 0.5f);
	img->dimentions.x = scale * time;
}

static void ScaleCard(Image *img, float time)
{
	float scale = 1.45f / (TURN_TIME * 0.5f);
	time = (time < 0.0f) ? 0.0f : time;
	img->dimentions.x = 1.45f - scale * time;
}

void BjDeck::CardTurning(int index)
{
	if (cards[index].turnTime <= 0.0f)
		return ;
	cards[index].turnTime -= DeltaTime();
	float t = cards[index].turnTime;
	if (t >= (TURN_TIME * 0.5f))
		FlattenCard(cards[index].image, t - (TURN_TIME * 0.5f));
	else
	{
		if (cards[index].faceUp == true)
			cards[index].image->SetTexture(cardTexts[cards[index].suit][cards[index].number]);
		else
			cards[index].image->SetTexture("cardBack");
		ScaleCard(cards[index].image, t);
	}
}

void BjDeck::MoveCard(int index)
{
	if (cards[index].movePos.x < -30.0f)
		return ;
	t_Point pos = cards[index].image->position;
	t_Point dest = cards[index].movePos;
	t_Point dir = {dest.x - pos.x, dest.y - pos.y};
	t_Point unitV = VectorNormalize(dir);
	float mag = VectorMagnitude(dir);
	pos = {pos.x + unitV.x * MOVE_SPEED * DeltaTime(), pos.y + unitV.y * MOVE_SPEED * DeltaTime()};
	dir = {dest.x - pos.x, dest.y - pos.y};
	if (mag < VectorMagnitude(dir))
	{
		cards[index].image->position = cards[index].movePos;
		cards[index].movePos.x = -31.0f;
		return ;
	}
	cards[index].image->position = pos;
}

void BjDeck::PositionCards(t_Point pos, int index)
{
	cards[index].movePos = pos;
}

Cards *BjDeck::TakeCard(int *index)
{
	if (cards[currentCardIndex].suit == (-1))
	{
		PositionCards({cards[currentCardIndex].image->position.x, 12.0f}, currentCardIndex);
		needToSuffle = true;
		currentCardIndex += 1;
	}
	*index = currentCardIndex;
	currentCardIndex += 1;
	return (&cards[*index]);
}

void BjDeck::SuffleCards()
{
	needToSuffle = false;
	suffling = true;
	suffleTime = 0.68f;
	for (int i = 0; i < DECK_SIZE; i++)
	{
		if (cards[i].used)
			continue ;
		PositionCards({cards[i].image->position.x, 12.0f}, i);
	}
}

void BjDeck::SuffleCheck()
{
	bool possed = true;
	for (int i = 0; i < DECK_SIZE; i++)
	{
		if (cards[i].movePos.x > -30.9f && cards[i].movePos.y > -30.9f)
		{
			possed = false;
			break ;
		}
	}
	if (!possed)
		return ;
	int blank = rand() % 20 + (DECK_SIZE * 0.58f);
	float xScale = 2.8f / DECK_SIZE;
	for (int i = 0; i < DECK_SIZE; i++)
	{
		cards[i].used = false;
		cards[i].turnTime = 0.0f;
		cards[i].faceUp = false;
		if (i == blank)
		{
			cards[i].image->SetTexture("cardBlank");
			cards[i].image->position = {X_POS + xScale * i + 0.1f, 12.0f - 0.08f};
			cards[i].movePos = {X_POS + xScale * i + 0.1f, Y_POS - 0.08f};
			cards[i].image->SetColor(0.788f * 0.8, 0.627f * 0.8, 0.03f * 0.8, 1.0f);
			cards[i].image->drawDepth = -(float)i;
			cards[i].value = -1;
			cards[i].suit = -1;
			cards[i].number = -1;
			continue ;
		}
		cards[i].image->SetTexture("cardBack");
		cards[i].image->position = {X_POS + xScale * i, 12.0f};
		cards[i].movePos = {X_POS + xScale * i, Y_POS};
		cards[i].image->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
		cards[i].image->drawDepth = -(float)i;
	}
	SuffleValues();
}

void BjDeck::Update()
{
	if (suffleTime <= 0.0f)
		suffling = false;
	else
	{
		SuffleCheck();
		suffling = true;
		suffleTime -= DeltaTime();
	}
	if (needToSuffle)
		SuffleCards();
	for (int i = 0; i < DECK_SIZE; i++)
	{
		CardTurning(i);
		MoveCard(i);
	}
}

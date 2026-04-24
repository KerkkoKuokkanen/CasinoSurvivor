
#ifndef BJ_DECK_H
#define BJ_DECK_H

#include "component.h"

#define DECK_SIZE 313

struct Cards {
	Image *image;
	int number;
	int value;
	int suit;
	float turnTime;
	bool faceUp;
	t_Point movePos;
	bool used;
};

class BjDeck : public CustomComponent
{
	private:
		Cards cards[DECK_SIZE] = {0};
		void SuffleValues();
		void CardTurning(int index);
		void MoveCard(int index);
		void SuffleCheck();
		int currentCardIndex = 0;
		float suffleTime = 0.0f;
	public:
		bool suffling = false;
		bool needToSuffle = false;
		void SuffleCards();
		void TurnCard(int index);
		void PositionCards(t_Point pos, int index);
		Cards *TakeCard(int *index);
		BjDeck();
		~BjDeck();
		void Update() override;
};

REGISTER_COMPONENT(BjDeck);

#endif


#ifndef BJ_DECK_H
#define BJ_DECK_H

#include "component.h"

#define DECK_SIZE 313

struct Cards {
	Image *image;
	int number;
	int value;
	int suit;
};

class BjDeck : public CustomComponent
{
	private:
		Cards cards[DECK_SIZE] = {0};
		void SuffleValues();
	public:
		BjDeck();
		~BjDeck();
};

REGISTER_COMPONENT(BjDeck);

#endif


#ifndef BJ_CARD_DEALING_H
# define BJ_CARD_DEALING_H

# include "component.h"
# include "BjDeck.h"
# include "bjBetter.h"

struct DealQueue {
	bool player;
	bool turn;
	bool ordered;
	t_Point pos;
	Cards *card;
	int index;
	int stack;
	float time;
};

class BjDealing : public CustomComponent
{
	private:
		int currDeck = 0;
		Image *currentDeck = NULL;
		Image *buttons[4] = {NULL, NULL, NULL, NULL};
		BjDeck *deck = NULL;
		BjBetter *bet = NULL;
		std::queue<DealQueue> dealQueue;
		std::vector<DealQueue> usedCards;
		DealQueue currentDeal = {false, false, false, {0.0f, 0.0f}, NULL, -1, -1.0f};
		void ResetDealing();
		void GetDealtCard();
		void UpdateDealtCard();
		void CheckButtons();
		void HandlePlaying();
		void Discard();
		void CheckWins();
		float order = -10.0f;
		bool inVoid = false;
	public:
		~BjDealing();
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(BjDealing);

std::vector<DealQueue> GetStackCards(int stack, std::vector<DealQueue> &used, DealQueue &curr);
std::tuple<int, int> CheckInitialCards(DealQueue &one, DealQueue &two, DealQueue &opp);
bool CheckBJ(DealQueue &one, DealQueue &two);
bool CheckSplit(DealQueue &one, DealQueue &two);

#endif

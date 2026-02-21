
#ifndef BJ_BETS_H
#define BJ_BETS_H

#include "component.h"
#include "number.h"

class BjBets : public CustomComponent
{
	private:
		Image *bet = NULL;
		Image *pp = NULL;
		Image *combo = NULL;
		Image *hovered = NULL;
		int betAmounts[3] = {0, 0, 0};
		Number *numBet = NULL;
		Number *numPP = NULL;
		Number *numCombo = NULL;
		void ManageNumber();
		void DeleteNums();
	public:
		bool active = false;
		int chosen = -1;
		BjBets();
		~BjBets();
		int GetBetAmounts() {return (betAmounts[0] + betAmounts[1] + betAmounts[2]);};
		void ResetBetAmounts();
		void Update() override;
};

REGISTER_COMPONENT(BjBets);

#endif

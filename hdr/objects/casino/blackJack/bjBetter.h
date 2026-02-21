
#ifndef BJ_BETTER_H
# define BJ_BETTER_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"
# include "button.h"
# include "BjBets.h"

class BjBetter : public CustomComponent
{
	private:
		BjBets *bets = NULL;
		Image *chip1 = NULL;
		Image *chip5 = NULL;
		Image *chip10 = NULL;
		Image *chip50 = NULL;
		Image *chip100 = NULL;
		Image *chip500 = NULL;
		Image *bet = NULL;
		Image *clear = NULL;
		Image *imgs[8] = {0};
		Button buttons[8];
		bool active = true;
		void ManageChips();
		void ManageButtons();
		void ManageRound();
		float roundStartTime = 1.4f;
		int chosen = -1;
	public:
		bool roundActive = false;
		BjBetter();
		~BjBetter();
		void Start() override;
		void Update() override;
		void Activate();
		void Deactivate();
};

REGISTER_COMPONENT(BjBetter);

#endif


#ifndef BJ_BETTER_H
# define BJ_BETTER_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"
# include "button.h"

class BjBetter : public CustomComponent
{
	private:
		Image *chip1 = NULL;
		Image *chip5 = NULL;
		Image *chip10 = NULL;
		Image *chip50 = NULL;
		Image *chip100 = NULL;
		Image *chip500 = NULL;
		Image *bet = NULL;
		Image *clear = NULL;
		Button buttons[8];
		bool active = true;
		uint64_t chipsBet = 0;
		void ManageChips();
		void ManageButtons();
	public:
		BjBetter();
		~BjBetter();
		void Update() override;
		void Activate();
		void Deactivate();
};

REGISTER_COMPONENT(BjBetter);

#endif

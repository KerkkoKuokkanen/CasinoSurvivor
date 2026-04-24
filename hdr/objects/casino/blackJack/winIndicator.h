
#ifndef WIN_INDICATOR_H
# define WIN_INDICATOR_H

# include "component.h"
# include "number.h"

class BjWinIndicator : public CustomComponent
{
	private:
		Number *amount = NULL;
		float lifeTime = 2.0f;
	public:
		void Update() override;
		void SetUp(int amount);
		~BjWinIndicator();
};

REGISTER_COMPONENT(BjWinIndicator);

#endif

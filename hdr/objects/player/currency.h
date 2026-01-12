
#ifndef CURRENCY_H
# define CURRENCY_H

# include "component.h"
# include "number.h"

class Currency : public CustomComponent
{
	private:
		Image *chips = NULL;
		Number *chipNumber = NULL;
		int chipAmount = 0;
		int addNum = 0;
		float numAddTime = 0.0f;
		float numAddTimer = 0.0f;
		void GetAddNum();
	public:
		Currency();
		~Currency();
		void Update() override;
};

REGISTER_COMPONENT(Currency);

#endif

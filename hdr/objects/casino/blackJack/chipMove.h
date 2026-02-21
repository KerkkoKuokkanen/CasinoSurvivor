
#ifndef CHIP_MOVE_H
# define CHIP_MOVE_H

# include "component.h"

class ChipMove : public CustomComponent
{
	private:
		Image *img = NULL;
		t_Point dest = {0.0f, 0.0f};
		t_Point dir = {1.0f, 0.0f};
	public:
		ChipMove();
		~ChipMove();
		void SetUp(int chip, int destination);
		void Update() override;
};

REGISTER_COMPONENT(ChipMove);

#endif

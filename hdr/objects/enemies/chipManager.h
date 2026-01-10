
#ifndef CHIP_MANAGER_H
# define CHIP_MANAGER_H

# include "component.h"
# include "player.h"
# include "particles.h"

#define MAX_CHIPS 10000

struct Chip {
	Image *img;
	t_Point pos;
	t_Point direction;
	float yAdd;
	float speed;
	int amount;
	bool collected;
	float collectTime;
	t_Box hitBox;
	float chipLifeTime;
	float dragTime;
	bool dragged;
	bool done;
};

class ChipManager : public CustomComponent
{
	private:
		uint64_t soundKey = 0;
		PlayerMovement *player = NULL;
		Particles *part = NULL;
		std::vector<Chip> chips = {};
		std::vector<int> GetCreatedChips(int amount);
		float dragInTime = 0.0f;
		void UpdateChips();
	public:
		~ChipManager();
		void CreateChips(t_Point pos, int amount);
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(ChipManager);

#endif

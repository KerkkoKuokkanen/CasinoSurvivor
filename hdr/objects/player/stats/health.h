
#ifndef HEALTH_H
# define HEALTH_H

# include "component.h"
# include "number.h"

class Health : public CustomComponent
{
	private:
		uint64_t soundKey[4] = {0, 0, 0, 0};
		Image *pill = NULL;
		Number *num = NULL;
		int health = 0;
		float numberAddTime = 0.0f;
		float numAddTime = 0.0f;
		float numShakeTime = 0.0f;
		float numShakeMini = 0.05f;
		bool cycle2 = false;
		t_Point numPos = {0.0f, 0.0f};
		float numberShakeMag = 0.0f;
		void Hfunc1();
		void Hfunc2();
	public:
		Health();
		~Health();
		void Damage(int damage);
		void Update() override;
};

REGISTER_COMPONENT(Health);

#endif

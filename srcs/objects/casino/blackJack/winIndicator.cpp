
#include "winIndicator.h"

static float CN(int n)
{
	return (float)n / 255.0f;
}

void BjWinIndicator::Update()
{
	if (lifeTime <= 0.0f)
	{
		RemoveSelf();
		return ;
	}
	if (lifeTime <= 0.6f)
	{
		float a = amount->GetColor().h;
		if (a > 0.001f)
			a = fmax(0.0f, a - DeltaTime() * 4.0f);
		t_Box c2 = amount->GetColor();
		amount->SetColor(c2.x, c2.y, c2.w, a);
		lifeTime -= DeltaTime();
		return ;
	}
	float a = amount->GetColor().h;
	if (a < 0.999f)
		a = fmin(1.0f, a + DeltaTime() * 4.0f);
	t_Box c1 = amount->GetColor();
	amount->SetColor(c1.x, c1.y, c1.w, a);
	lifeTime -= DeltaTime();
}

void BjWinIndicator::SetUp(int amount)
{
	int a = abs(amount);
	t_Box color = {CN(21), CN(209), CN(79), 0.0f};
	if (amount < 0)
		color = {CN(191), CN(33), CN(19), 0.0f};
	BjWinIndicator::amount = new Number({0.0f, -0.5f}, std::to_string(a), 1.0f, color, 1.0f);
	float pos = 0.5f - (float)std::to_string(a).length() * 0.5f;
	BjWinIndicator::amount->SetPosition({pos, -0.5f});
}

BjWinIndicator::~BjWinIndicator()
{
	if (amount != NULL)
		delete amount;
}

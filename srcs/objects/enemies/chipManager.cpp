
#include "chipManager.h"
#include "audio.h"

#define DRAG_IN_TIME 0.6f
#define DAMPING std::pow(0.9, 60)
#define CHIP_LIFE_TIME 12.0f
//chips: 1, 2, 5, 10, 25, 50, 100, 200, 500

static bool HitBoxCheck(t_Box one, t_Box two)
{
	bool x = (one.x + one.w > two.x) && (one.x < two.x + two.w);
	bool y = (one.y + one.h > two.y) && (one.y < two.y + two.h);
	return (x && y);
}

static float GetChipRect(int chip)
{
	switch (chip)
	{
		case 1:
			return 0.0f;
		case 2:
			return 0.1111f;
		case 5:
			return 0.2222f;
		case 10:
			return 0.3333f;
		case 25:
			return 0.4444f;
		case 50:
			return 0.5555f;
		case 100:
			return 0.6666f;
		case 200:
			return 0.7777f;
		case 500:
			return 0.8888f;
	}
	return 0.0f;
}

ChipManager::~ChipManager()
{
	for (int i = 0; i < chips.size(); i++)
		delete chips[i].img;
}

void ChipManager::Start()
{
	chips.reserve(MAX_CHIPS);
	SystemObj *obj = FindSystemObject(2830363874434601420LU);
	player = (PlayerMovement*)obj->GetComponent("PlayerMovement");
	obj = FindSystemObject(10158550851708924347LU);
	part = (Particles*)obj->GetComponent("Particles");
}

std::vector<int> ChipManager::GetCreatedChips(int amount)
{
	std::vector<int> chipsCreated = {};
	int chipList[9] = {500, 200, 100, 50, 25, 10, 5, 2, 1};
	for (int i = 0; i < 9; i++)
	{
		while (amount >= chipList[i])
		{
			chipsCreated.push_back(chipList[i]);
			amount -= chipList[i];
		}
	}
	return (chipsCreated);
}

void ChipManager::CreateChips(t_Point pos, int amount)
{
	std::vector<int> chipsCreated = GetCreatedChips(amount);
	int angle = rand() % 360;
	int add = 360 / (int)chipsCreated.size();
	for (int i = 0; i < chipsCreated.size(); i++)
	{
		if (chips.size() >= MAX_CHIPS)
			return ;
		float xAdd = float_rand() * 0.1f * (rand() % 2 == 0 ? 1.0f : -1.0f);
		float yAdd = float_rand() * 0.1f * (rand() % 2 == 0 ? 1.0f : -1.0f);
		float yDiff = -0.3f + float_rand();
		float sSin = GetSinAngle(angle);
		float cCos = GetCosAngle(angle);
		float srcsX = GetChipRect(chipsCreated[i]);
		float angleRand = float_rand() * PI * 0.1f * (rand() % 2 == 0 ? 1.0f : -1.0f);
		float speed = 2.0f + float_rand() * 2.0f;
		float dirX = 1.0f * cCos;
		float dirY = 1.0f * sSin;
		float hbDim = 0.1f + float_rand() * 0.4f;
		t_Box hitBox = {0.0f, 0.0f, hbDim, hbDim};
		angle += add;
		Image *img = new Image("chipSheet", {pos.x + xAdd, pos.y + yAdd, 0.65f, 0.65f}, angleRand, 7);
		img->drawDepth = -10.0f;
		img->SetTextureData(srcsX, 0.0f, 0.1111f, 1.0f, 0.0f);
		img->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
		hitBox.x = img->position.x - hitBox.w * 0.5f;
		hitBox.y = img->position.y - hitBox.w * 0.5f;
		chips.push_back({img, img->position, {dirX, dirY}, yDiff, speed, chipsCreated[i], false, 0.25f, hitBox, CHIP_LIFE_TIME, DRAG_IN_TIME, false, false});
	}
}

void ChipManager::UpdateChips()
{
	std::vector<t_Box> colors = {{0.8f, 0.8f, 0.8f, 0.9f}, {0.7f, 0.7f, 0.7f, 0.9f}, {0.9f, 0.9f, 0.9f, 0.9f}};
	bool shooting = player->shooting;
	float alphaScale = 1.0f / 0.125f;
	t_Box hb = player->GetHitBox();
	for (int i = 0; i < chips.size(); i++)
	{
		if (chips[i].collected)
		{
			float alpha = fmin(1.0f, alphaScale * chips[i].collectTime);
			chips[i].img->position.y += 0.5f * DeltaTime();
			chips[i].img->SetColor(0.8f, 0.8f, 0.8f, alpha);
			chips[i].collectTime -= DeltaTime();
			if (chips[i].collectTime <= 0.0f)
			{
				chips[i].done = true;
				delete chips[i].img;
			}
			continue ;
		}
		if (!shooting && chips[i].dragged == false)
		{
			chips[i].dragTime -= DeltaTime();
			if (chips[i].dragTime <= 0.0f)
			{
				part->AddParticles(chips[i].pos, 0.1f, {1.0f, 0.f}, PI * 0.5f, colors, 1.0f, 2.0f, 16, "particle", 0.1f, 0.125f, 0.2f, 0.25f, 0.0f);
				chips[i].dragged = true;
			}
		}
		else
			chips[i].dragTime = DRAG_IN_TIME;
		if (chips[i].dragged)
		{
			t_Point pos = player->GetPosition();
			t_Point dir = {pos.x - chips[i].pos.x, pos.y + chips[i].yAdd - chips[i].pos.y};
			dir = VectorNormalize(dir);
			chips[i].direction.x += dir.x * DeltaTime() * 16.0f;
			chips[i].direction.y += dir.y * DeltaTime() * 16.0f;
			chips[i].direction = VectorNormalize(chips[i].direction);
			chips[i].speed = fmin(chips[i].speed + DeltaTime() * 46.0f, 20.0f);
			t_Box col = chips[i].img->GetColor();
			if (col.h < 1.0f)
				col.h += 4.0f * DeltaTime();
			chips[i].img->SetColor(col.x, col.y, col.w, col.h);
		}
		else
		{
			chips[i].speed *= std::pow(DAMPING, DeltaTime());
			if (fabs(chips[i].speed) < 0.001f)
			{
				chips[i].speed = 0.0f;
				chips[i].direction = {0.0f, 0.0f};
			}
		}
		chips[i].pos.x = chips[i].pos.x + chips[i].direction.x * chips[i].speed * DeltaTime();
		chips[i].pos.y = chips[i].pos.y + chips[i].direction.y * chips[i].speed * DeltaTime();
		chips[i].img->position = chips[i].pos;
		chips[i].hitBox.x = chips[i].img->position.x - chips[i].hitBox.w * 0.5f;
		chips[i].hitBox.y = chips[i].img->position.y - chips[i].hitBox.w * 0.5f;
		if (HitBoxCheck(chips[i].hitBox, hb))
		{
			soundKey = RePlaySound("chip", 12.0f, 0, soundKey);
			chips[i].collected = true;
			chips[i].img->SetColor(0.8f, 0.8f, 0.8f, 1.0f);
			continue ;
		}
		if (chips[i].chipLifeTime < 3.0f && chips[i].dragged == false)
		{
			float alp = cos(7.3f * chips[i].chipLifeTime + PI) + 1.0f;
			chips[i].img->SetColor(0.8f, 0.8f, 0.8f, alp);
		}
		if (chips[i].chipLifeTime <= 0.0f)
		{
			delete chips[i].img;
			chips[i].done = true;
		}
		if (chips[i].dragged == false)
			chips[i].chipLifeTime -= DeltaTime();
	}
	auto new_end = std::remove_if(
		chips.begin(), 
		chips.end(), 
		[](const Chip chip) {
			return chip.done; 
		}
	);
	chips.erase(new_end, chips.end());
}

void ChipManager::Update()
{
	UpdateChips();
}

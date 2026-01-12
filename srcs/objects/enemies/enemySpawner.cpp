
#include "enemySpawner.h"
#include "deltaTime.h"
#include "envHandler.h"
#include "exit.h"

static bool HitBoxCheck(t_Box one, t_Box two)
{
	bool x = (one.x + one.w > two.x) && (one.x < two.x + two.w);
	bool y = (one.y + one.h > two.y) && (one.y < two.y + two.h);
	return (x && y);
}

EnemySpawner::~EnemySpawner()
{
	for (int i = 0; i < enemies.size(); i++)
		delete enemies[i];
	enemies.clear();
}

void EnemySpawner::SetTime()
{
	time += (double)DeltaTime();
}

void EnemySpawner::SetEnemyQueue(std::vector<EnemyData> &queue)
{
	for (int i = 0; i < queue.size(); i++)
		enemyQueue.push(queue[i]);
}

void EnemySpawner::SpawnEnemies()
{
	if (enemies.size() >= MAX_ENEMIES)
		return ;
	while (enemyQueue.size() > 0)
	{
		EnemyData &data = enemyQueue.front();
		if (data.time > time)
			break ;
		std::tuple<t_Point, float, bool> enem = FindPosAndSize(data);
		if (!std::get<2>(enem))
			break ;
		CommonEnemy *e = EnemyCasting(data, std::get<0>(enem), std::get<1>(enem));
		if (e != NULL && data.independent == false)
		{
			e->currency = data.currency;
			e->damage = data.damage;
			e->health = data.health;
			e->speed = data.speed;
			e->type = data.type;
		}
		enemyQueue.pop();
	}
}

static bool SortVect(EnemyData &one, EnemyData &two)
{
	return (one.time < two.time);
}

void EnemySpawner::Start()
{
	SystemObj *obj = FindSystemObject(17206662188527305259LU);
	grid = (FloorGrid*)obj->GetComponent("FloorGrid");
	shake = (CameraShake*)self->GetComponent("CameraShake");
	obj = FindSystemObject(2830363874434601420LU);
	player = (PlayerMovement*)obj->GetComponent("PlayerMovement");
	enemies.reserve(MAX_ENEMIES);
	time = 0.0;
	std::vector<EnemyData> enems;
	for (int i = 0; i < 1; i++)
	{
		EnemyData add;
		add.health = 12;
		add.damage = 4;
		add.currency = 3;
		add.independent = false;
		add.speed = 1.0f;
		add.time = float_rand() * 1.0f;
		add.type = 1;
		add.size = 2.0f;
		enems.push_back(add);
	}
	std::sort(enems.begin(), enems.end(), SortVect);
	SetEnemyQueue(enems);
}

void EnemySpawner::UpdateEnemies()
{
	bool playerNotHit = player->canHit;
	t_Box hb = player->GetHitBox();
	float rightMost = -999.0f;
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->Update();
		rightMost = fmax(enemies[i]->position.x, rightMost);
		if (enemies[i]->independent)
		{
			if (enemies[i]->active == false)
			{
				delete enemies[i];
				enemies[i] = NULL;
			}
			continue ;
		}
		float x = enemies[i]->position.x;
		if (enemies[i]->health > 0 && playerNotHit && HitBoxCheck(hb, enemies[i]->hitbox))
		{
			int dmg = 4;
			playerNotHit = false;
			SetDeltaMulti(0.02f);
			deltaTimer = 0.15f * (1.0f + (1.4f / 26.0f) * fmin(26.0f, (float)(dmg - 4)));
			grid->ApplyForce(enemies[i]->position, 0.2f, 0.5f);
			shake->CreateCameraShake(0.05f);
			Invoke([=]() { shake->CreateCameraShakeDefault(); }, 4, 0.05f);
			enemies[i]->Attack();
			player->Damage(dmg, 25.0f, enemies[i]->position);
		}
		if (enemies[i]->health > 0)
			enemies[i]->Position(x - enemies[i]->speed * DeltaTime(), enemies[i]->position.y);
		if (enemies[i]->position.x + enemies[i]->hitbox.w < -12.5f || enemies[i]->active == false)
		{
			delete enemies[i];
			enemies[i] = NULL;
		}
	}
	auto new_end = std::remove_if(
		enemies.begin(), 
		enemies.end(), 
		[](const CommonEnemy *ptr) {
			return ptr == NULL; 
		}
	);
	if (deltaTimer > 0.0f)
		deltaTimer -= DeltaTimeReal();
	else
		SetDeltaMulti(1.0f);
	enemies.erase(new_end, enemies.end());
	if (rightMost < -7.2f && enemyQueue.size() == 0 && exitSpawned == false)
	{
		exitSpawned = true;
		enemies.push_back(new ExitEnemy());
	}
}

void EnemySpawner::Update()
{
	SetTime();
	SpawnEnemies();
	UpdateEnemies();
}

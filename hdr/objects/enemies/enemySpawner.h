
#ifndef ENEMY_SPAWNER_H
# define ENEMY_SPAWNER_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "enemyData.h"
# include "commonEnemy.h"
# include "camShake.h"
# include "floorGrid.h"
# include "player.h"
# include <queue>

# define MAX_ENEMIES 3000

class EnemySpawner : public CustomComponent
{
	private:
		bool exitSpawned = false;
		std::queue<EnemyData> enemyQueue = {};
		std::vector<CommonEnemy*> enemies = {};
		CameraShake *shake = NULL;
		FloorGrid *grid = NULL;
		PlayerMovement *player = NULL;
		double time = 0.0;
		float deltaTimer = 0.0f;
		void SetTime();
		void SpawnEnemies();
		void SetEnemyQueue(std::vector<EnemyData> &queue);
		CommonEnemy *EnemyCasting(EnemyData &data, t_Point pos, float size);
		void UpdateEnemies();
		std::tuple<t_Point, float, bool> FindPosAndSize(EnemyData &data);
	public:
		~EnemySpawner();
		void Start() override;
		void Update() override;
		std::vector<CommonEnemy*> &GetEnemies() {return (enemies);};
};

REGISTER_COMPONENT(EnemySpawner);

#endif

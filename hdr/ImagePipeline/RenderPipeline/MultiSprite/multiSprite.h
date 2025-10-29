
#ifndef MULTI_SPRITE_H
# define MULTI_SPRITE_H

# include "IBO.h"
# include "renderSystem.h"

struct SpriteData {
	t_Point pos;
	t_Box sRect;
	t_Point dimentions;
	float angle;
	t_Box color;
};

class MultiSprite : public RenderObj
{
	private:
		float w = 1.0f;
		float h = 1.0f;
		IBO *ibo = NULL;
		uint32_t maxSize = 0;
		std::vector<uint32_t> freeKeys;
		std::vector<InstanceData> moddedData;
		std::vector<InstanceData> instances;
		void UpdateInstancesWithData();
		bool (*SortFunction)(InstanceData &one, InstanceData &two) = NULL;
	public:
		bool ySort = false;
		bool staticSprite = false;
		MultiSprite(GLuint texture, float widht, float height, uint32_t maxSize, int layer);
		~MultiSprite();
		uint32_t AddSprite(t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color);
		void SetDepth(float depth) {drawDepth = depth;};
		void SortingFunction(bool (*f)(InstanceData &one, InstanceData &two));
		void RemoveSprite(uint32_t key);
		void ModifySprite(uint32_t key, t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color);
		SpriteData GetSpriteData(uint32_t key);
		std::vector<std::tuple<uint32_t, SpriteData>> GetInstances();
		std::tuple<uint32_t, int, float, float> GetSavingData() {return {maxSize, layer, w, h};};
		void Draw() override;
};

#endif

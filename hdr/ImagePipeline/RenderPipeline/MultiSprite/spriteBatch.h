
#ifndef SPRITE_BATCH_H
# define SPRITE_BATCH_H

# include "multiSprite.h"
# include "customComponent.h"
# include "componentRegistry.h"

class SpriteBatch : public CustomComponent
{
	private:
		uint64_t hash = 0;
		float width = 1.0f;
		float height = 1.0f;
		uint32_t maxSize = 1000;
		int layer = 0;
		MultiSprite *multiSprite = NULL;
		bool (*SortFunction)(InstanceData &one, InstanceData &two) = NULL;
	public:
		~SpriteBatch();
		void SetUpMultiSprite(std::string texture, float widht, float height, uint32_t maxSize, int layer);
		void SetUpMultiSprite(uint64_t texture, float widht, float height, uint32_t maxSize, int layer);
		void SetSortingFunction(bool (*f)(InstanceData &one, InstanceData &two));
		uint32_t GetMaxSize() {return (maxSize);};
		uint32_t AddSprite(t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color);
		void SetDepth(float depth);
		void SetStaticSprite(bool stat) {multiSprite->staticSprite = stat;};
		void RemoveSprite(uint32_t key);
		void ModifySprite(uint32_t key, t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color);
		SpriteData GetSprite(uint32_t key);
		std::vector<std::tuple<uint32_t, SpriteData>> GetAllSprites();
};

REGISTER_COMPONENT(SpriteBatch);

#endif

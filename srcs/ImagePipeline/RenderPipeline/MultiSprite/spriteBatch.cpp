
#include "spriteBatch.h"
#include "Textures.h"

void SpriteBatch::SetUpMultiSprite(std::string texture, float widht, float height, uint32_t maxSize, int layer)
{
	t_Texture data = GetTextureGLData(texture);
	if (data.hash == 0)
		return ;
	if (multiSprite != NULL)
		delete multiSprite;
	multiSprite = new MultiSprite(data.text, widht, height, maxSize, layer);
	SpriteBatch::width = widht;
	SpriteBatch::height = height;
	SpriteBatch::hash = data.hash;
	SpriteBatch::maxSize = maxSize;
	SpriteBatch::layer = layer;
	multiSprite->SortingFunction(SortFunction);
}

void SpriteBatch::SetUpMultiSprite(uint64_t texture, float widht, float height, uint32_t maxSize, int layer)
{
	t_Texture tex = GetTextureGLData(texture);
	if (tex.hash == 0)
		return ;
	if (multiSprite != NULL)
		delete multiSprite;
	multiSprite = new MultiSprite(tex.text, widht, height, maxSize, layer);
	SpriteBatch::width = widht;
	SpriteBatch::height = height;
	SpriteBatch::hash = texture;
	SpriteBatch::maxSize = maxSize;
	SpriteBatch::layer = layer;
	multiSprite->SortingFunction(SortFunction);
}

void SpriteBatch::SetSortingFunction(bool (*f)(InstanceData &one, InstanceData &two))
{
	if (multiSprite == NULL)
		return ;
	SortFunction = f;
	multiSprite->SortingFunction(f);
}

SpriteBatch::~SpriteBatch()
{
	if (multiSprite != NULL)
		delete multiSprite;
}

uint32_t SpriteBatch::AddSprite(t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color)
{
	if (multiSprite == NULL)
		return (0);
	return (multiSprite->AddSprite(position, sRect, dimentions, angle, color));
}

void SpriteBatch::SetDepth(float depth)
{
	if (multiSprite == NULL)
		return ;
	multiSprite->SetDepth(depth);
}

void SpriteBatch::RemoveSprite(uint32_t key)
{
	if (multiSprite == NULL)
		return ;
	multiSprite->RemoveSprite(key);
}

void SpriteBatch::ModifySprite(uint32_t key, t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color)
{
	if (multiSprite == NULL)
		return ;
	multiSprite->ModifySprite(key, position, sRect, dimentions, angle, color);
}

SpriteData SpriteBatch::GetSprite(uint32_t key)
{
	if (multiSprite == NULL)
		return {};
	return (multiSprite->GetSpriteData(key));
}

std::vector<std::tuple<uint32_t, SpriteData>> SpriteBatch::GetAllSprites()
{
	if (multiSprite == NULL)
		return {};
	return (multiSprite->GetInstances());
}

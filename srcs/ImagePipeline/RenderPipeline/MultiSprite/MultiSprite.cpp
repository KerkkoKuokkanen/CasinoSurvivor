
#include "multiSprite.h"
#include "imageTransforms.h"

static bool MultiSpriteXSort(InstanceData &obj1, InstanceData &obj2)
{
	return (obj1.position.x > obj2.position.x);
}

static bool MultiSpriteYSort(InstanceData &obj1, InstanceData &obj2)
{
	if (std::fabs(obj1.position.y - obj2.position.y) < 0.0001f)
		return (MultiSpriteXSort(obj1, obj2));
	return (obj1.position.y > obj2.position.y);
}

MultiSprite::~MultiSprite()
{
	delete ibo;
}

MultiSprite::MultiSprite(GLuint texture, float widht, float height, uint32_t maxSize, int layer)
{
	renderType = 2;
	t_Point wh = TransformCoordinateToScreenSpace(widht, height);
	MultiSprite::maxSize = maxSize;
	instances.reserve(maxSize);
	moddedData.reserve(maxSize);
	freeKeys.reserve(maxSize);
	for (int i = 0; i < maxSize; i++)
	{
		freeKeys.push_back(i + 1);
		moddedData.push_back({0});
		moddedData[i].key = 0;
	}
	ibo = new IBO(maxSize, texture);
	w = wh.x;
	h = wh.y;
	AddToRenderSystem(layer);
}

void MultiSprite::SortingFunction(bool (*f)(InstanceData &one, InstanceData &two))
{
	SortFunction = f;
}

uint32_t MultiSprite::AddSprite(t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color)
{
	if (instances.size() >= maxSize)
		return (0);
	uint32_t key = freeKeys[freeKeys.size() - 1];
	freeKeys.pop_back();
	InstanceData first = {position, sRect, dimentions, angle, color, key, dimentions};
	instances.push_back(first);
	moddedData[key - 1] = {position, sRect, dimentions, angle, color, 0, dimentions};
	return (key);
}

void MultiSprite::RemoveSprite(uint32_t key)
{
	moddedData[key - 1].key = 2;
}

void MultiSprite::ModifySprite(uint32_t key, t_Point position, t_Box sRect, t_Point dimentions, float angle, t_Box color)
{
	moddedData[key - 1].key = 1;
	moddedData[key - 1].angle = angle;
	moddedData[key - 1].position = position;
	moddedData[key - 1].scale = dimentions;
	moddedData[key - 1].texUV = sRect;
	moddedData[key - 1].color = color;
	moddedData[key - 1].useScale = dimentions;
}

SpriteData MultiSprite::GetSpriteData(uint32_t key)
{
	SpriteData ret;
	ret.angle = moddedData[key - 1].angle;
	ret.dimentions = moddedData[key - 1].scale;
	ret.sRect = moddedData[key - 1].texUV;
	ret.color = moddedData[key - 1].color;
	ret.pos = moddedData[key - 1].position;
	return (ret);
}

void MultiSprite::UpdateInstancesWithData()
{
	for (int i = 0; i < instances.size(); i++)
	{
		uint32_t key = instances[i].key;
		if (moddedData[key - 1].key == 1)
		{
			instances[i] = moddedData[key - 1];
			instances[i].key = key;
			moddedData[key - 1].key = 0;
		}
		if (moddedData[key - 1].key == 2)
		{
			freeKeys.push_back(instances[i].key);
			instances[i].key = 0;
			continue ;
		}
		if (!staticSprite)
			instances[i].position = TransformCoordinateToScreenSpaceCamera(moddedData[key - 1].position.x, moddedData[key - 1].position.y);
		else
			instances[i].position = TransformCoordinateToScreenSpace(moddedData[key - 1].position.x, moddedData[key - 1].position.y);
	}
	instances.erase(
	std::remove_if(instances.begin(), instances.end(),
				[](const InstanceData& instance) {
					return instance.key == 0;
				}),
	instances.end());
	if (ySort)
		std::sort(instances.begin(), instances.end(), MultiSpriteYSort);
	else if (SortFunction != NULL)
		std::sort(instances.begin(), instances.end(), SortFunction);
}

std::vector<std::tuple<uint32_t, SpriteData>> MultiSprite::GetInstances()
{
	std::vector<std::tuple<uint32_t, SpriteData>> data = {};
	for (int i = 0; i < instances.size(); i++)
	{
		uint32_t key = instances[i].key;
		if (key == 0)
			continue ;
		SpriteData ret;
		ret.angle = moddedData[key - 1].angle;
		ret.dimentions = moddedData[key - 1].scale;
		ret.sRect = moddedData[key - 1].texUV;
		ret.color = moddedData[key - 1].color;
		ret.pos = moddedData[key - 1].position;
		data.push_back({key, ret});
	}
	return (data);
}

void MultiSprite::Draw()
{
	UpdateInstancesWithData();
	ibo->UpdateInstances(instances);
	ibo->Draw(w, h);
}

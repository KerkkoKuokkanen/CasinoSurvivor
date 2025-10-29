
#include "hitboxManager.h"
#include "vector"
#include "unordered_map"
#include "tuple"
#include "convexOverlap.h"

std::unordered_map<uint32_t, std::vector<std::tuple<uint32_t, Hitbox*>>> hitboxes = {};

static float HBoxGetLowY(t_BoundingB &data)
{
	float d_drawY = data.leftBottom.y;
	if (d_drawY > data.leftTop.y)
		d_drawY = data.leftTop.y;
	if (d_drawY > data.rightBottom.y)
		d_drawY = data.rightBottom.y;
	if (d_drawY > data.rightTop.y)
		d_drawY = data.rightTop.y;
	return (d_drawY);
}

static float HBoxGetHighY(t_BoundingB &data)
{
	float d_drawY = data.leftBottom.y;
	if (d_drawY < data.leftTop.y)
		d_drawY = data.leftTop.y;
	if (d_drawY < data.rightBottom.y)
		d_drawY = data.rightBottom.y;
	if (d_drawY < data.rightTop.y)
		d_drawY = data.rightTop.y;
	return (d_drawY);
}

static float HBoxGetLowX(t_BoundingB &data)
{
	float d_drawx = data.leftBottom.x;
	if (d_drawx > data.leftTop.x)
		d_drawx = data.leftTop.x;
	if (d_drawx > data.rightBottom.x)
		d_drawx = data.rightBottom.x;
	if (d_drawx > data.rightTop.x)
		d_drawx = data.rightTop.x;
	return (d_drawx);
}

static float HBoxGetHighX(t_BoundingB &data)
{
	float d_drawx = data.leftBottom.x;
	if (d_drawx < data.leftTop.x)
		d_drawx = data.leftTop.x;
	if (d_drawx < data.rightBottom.x)
		d_drawx = data.rightBottom.x;
	if (d_drawx < data.rightTop.x)
		d_drawx = data.rightTop.x;
	return (d_drawx);
}

void AddHitBox(Hitbox *hitbox)
{
	uint32_t key = hitbox->GetKey();
	uint32_t collisionNumber = hitbox->GetCollisionNumber();
	hitboxes[collisionNumber].push_back({key, hitbox});
}

void RemoveHitBox(Hitbox *hitbox)
{
	uint32_t key = hitbox->GetKey();
	uint32_t collisionNumber = hitbox->GetCollisionNumber();
	for (auto &[ukey, obj] : hitboxes[collisionNumber])
	{
		if (ukey == key)
		{
			ukey = 0;
			return ;
		}
	}
}

std::vector<std::tuple<Hitbox*, t_Point, float>> CheckCollisionRigidBody(Hitbox *hitbox)
{
	std::vector<std::tuple<Hitbox*, t_Point, float>> ret = {};
	t_BoundingB box = hitbox->GetHitBox();
	uint32_t uKey = hitbox->GetKey();
	for (auto &[colNum, vec] : hitboxes)
	{
		vec.erase(std::remove_if(vec.begin(), vec.end(),
					[](std::tuple<uint32_t, Hitbox*> data) {
						return std::get<0>(data) == 0;
					}), vec.end());
		if (std::find(hitbox->ignored.begin(), hitbox->ignored.end(), colNum) != hitbox->ignored.end())
			continue ;
		for (auto &[key, hBox] : vec)
		{
			if (key == uKey || key == 0)
				continue ;
			t_Point dir = {0.0f, 0.0f};
			float depth = 0.0f;
			if (ConvexOverlap2(box, hBox->GetHitBox(), depth, dir))
				ret.push_back({hBox, dir, depth});
		}
	}
	return (ret);
}

std::vector<Hitbox*> CheckCollision(Hitbox *hitbox)
{
	std::vector<Hitbox*> ret = {};
	t_BoundingB box = hitbox->GetHitBox();
	uint32_t uKey = hitbox->GetKey();
	for (auto &[colNum, vec] : hitboxes)
	{
		vec.erase(std::remove_if(vec.begin(), vec.end(),
					[](std::tuple<uint32_t, Hitbox*> data) {
						return std::get<0>(data) == 0;
					}), vec.end());
		if (std::find(hitbox->ignored.begin(), hitbox->ignored.end(), colNum) != hitbox->ignored.end())
			continue ;
		for (auto &[key, hBox] : vec)
		{
			if (key == uKey || key == 0)
				continue ;
			if (ConvexOverlap(box, hBox->GetHitBox()))
				ret.push_back(hBox);
		}
	}
	return (ret);
}

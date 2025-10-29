
#include "hitbox.h"
#include "hitboxManager.h"
#include "lineDrawing.h"
#include "camera.h"
#include "vectorTools.h"

static uint32_t HitBoxKeyGen()
{
	static uint32_t keys = 0;
	keys += 1;
	if (keys > 4294967292)
		keys = 1;
	return (keys);
}

static int ignoredCollision = -1;

Hitbox::Hitbox()
{
	key = HitBoxKeyGen();
	AddHitBox(this);
}

Hitbox::~Hitbox()
{
	RemoveHitBox(this);
}

void Hitbox::Init(void *data, size_t size)
{
	AddToSaveTracking(&pos.x, sizeof(float));
	AddToSaveTracking(&pos.y, sizeof(float));
	AddToSaveTracking(&dims.x, sizeof(float));
	AddToSaveTracking(&dims.y, sizeof(float));
	AddToSaveTracking(&angle, sizeof(float));
	AddToSaveTracking(&collisionNumber, sizeof(uint16_t));
	CreateInputField("Pos X", n_VarType::FLOAT, &pos.x);
	CreateInputField("Pos Y", n_VarType::FLOAT, &pos.y);
	CreateInputField("Dim X", n_VarType::FLOAT, &dims.x);
	CreateInputField("Dim Y", n_VarType::FLOAT, &dims.y);
	CreateInputField("Angle", n_VarType::FLOAT, &angle);
	CreateInputField("Collision Num", n_VarType::INTEGER, &collisionNumber);
	CreateInputField("Ignored Col", n_VarType::INTEGER, &ignoredCollision);
	CreateInputField("Apply", n_VarType::BOOL, &apply);
	if (size <= 0)
		return ;
	char *cast = (char*)data;
	pos.x = READ_AND_ADVANCE(cast, float);
	pos.y = READ_AND_ADVANCE(cast, float);
	dims.x = READ_AND_ADVANCE(cast, float);
	dims.y = READ_AND_ADVANCE(cast, float);
	angle = READ_AND_ADVANCE(cast, float);
	collisionNumber = READ_AND_ADVANCE(cast, uint16_t);
	size_t offset = sizeof(float) * 5 + sizeof(uint16_t);
	while (offset < size)
	{
		uint32_t used = READ_AND_ADVANCE(cast, uint32_t);
		ignored.push_back(used);
		offset += sizeof(uint32_t);
	}
	SetUpOwn();
}

void Hitbox::SetUpHitbox(t_Point pos, t_Point dims, float angle)
{
	Hitbox::pos = pos;
	Hitbox::dims = dims;
	Hitbox::angle = angle;
	dims.x *= 0.5f;
	dims.y *= 0.5f;
	hitbox.leftBottom = VectorRotate({-dims.x, -dims.y}, angle);
	hitbox.leftTop = VectorRotate({-dims.x, dims.y}, angle);
	hitbox.rightBottom = VectorRotate({dims.x, -dims.y}, angle);
	hitbox.rightTop = VectorRotate({dims.x, dims.y}, angle);
	hitbox.leftBottom.x += pos.x;
	hitbox.leftBottom.y += pos.y;
	hitbox.leftTop.x += pos.x;
	hitbox.leftTop.y += pos.y;
	hitbox.rightBottom.x += pos.x;
	hitbox.rightBottom.y += pos.y;
	hitbox.rightTop.x += pos.x;
	hitbox.rightTop.y += pos.y;
}

void Hitbox::Angle(float angle)
{
	SetUpHitbox(pos, dims, angle);
}

void Hitbox::Position(t_Point pos)
{
	SetUpHitbox(pos, dims, angle);
}

void Hitbox::Dimentions(t_Point dims)
{
	SetUpHitbox(pos, dims, angle);
}

void Hitbox::SetUpOwn()
{
	SetUpHitbox(pos, dims, angle);
}

std::vector<Hitbox*> Hitbox::Collision()
{
	return (CheckCollision(this));
}

void Hitbox::SetCollisionNumber(uint32_t collisionNumber)
{
	Hitbox::collisionNumber = collisionNumber;
	RemoveHitBox(this);
	AddHitBox(this);
}

void Hitbox::IgnoreCollisionNumber(uint32_t collision)
{
	ClearToSave();
	ignored.push_back(collision);
	AddToSave(ignored.data(), sizeof(uint32_t) * ignored.size());
}

void Hitbox::ClearIgnoredCollisions()
{
	ClearToSave();
	ignored.clear();
}

void Hitbox::LastUpdate()
{
	std::vector<t_Point> points = {};
	points.push_back(TransformCoordinateToScreenSpaceCamera(hitbox.leftBottom.x, hitbox.leftBottom.y));
	points.push_back(TransformCoordinateToScreenSpaceCamera(hitbox.leftTop.x, hitbox.leftTop.y));
	points.push_back(TransformCoordinateToScreenSpaceCamera(hitbox.rightTop.x, hitbox.rightTop.y));
	points.push_back(TransformCoordinateToScreenSpaceCamera(hitbox.rightBottom.x, hitbox.rightBottom.y));
	points.push_back(TransformCoordinateToScreenSpaceCamera(hitbox.leftBottom.x, hitbox.leftBottom.y));
	DrawLinesWithColor(points, {1.0f, 1.0f, 1.0f, 1.0f});
}

void Hitbox::EngineUpdate()
{
	SetUpOwn();
	if (apply)
	{
		SetCollisionNumber(collisionNumber);
		if (ignoredCollision >= 0)
		{
			IgnoreCollisionNumber(ignoredCollision);
			ignoredCollision = -1;
		}
	}
	apply = false;
	std::vector<t_Point> points = {};
	points.push_back(TransformCoordinateToScreenSpaceCamera(hitbox.leftBottom.x, hitbox.leftBottom.y));
	points.push_back(TransformCoordinateToScreenSpaceCamera(hitbox.leftTop.x, hitbox.leftTop.y));
	points.push_back(TransformCoordinateToScreenSpaceCamera(hitbox.rightTop.x, hitbox.rightTop.y));
	points.push_back(TransformCoordinateToScreenSpaceCamera(hitbox.rightBottom.x, hitbox.rightBottom.y));
	points.push_back(TransformCoordinateToScreenSpaceCamera(hitbox.leftBottom.x, hitbox.leftBottom.y));
	DrawLinesWithColor(points, {1.0f, 1.0f, 1.0f, 1.0f});
}

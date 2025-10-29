
#include "rigidBody.h"
#include "rigidBodyManager.h"
#include "hitbox.h"
#include "keyboard.h"
#include "vectorTools.h"
#include "deltaTime.h"
#include "optional"

#define RIGIDBODY_LOOPS 16

static uint32_t RigidBodyKeyGen()
{
	static uint32_t keys = 0;
	keys += 1;
	if (keys > 4294967292)
		keys = 1;
	return (keys);
}

RigidBody::~RigidBody()
{
	RemoveRigidBody(this);
}

void RigidBody::Init(void *data, size_t size)
{
	ownKey = RigidBodyKeyGen();
	if (size <= 0)
		return ;
}

void RigidBody::Start()
{
	AddRigidBody(this);
	Hitbox *used = (Hitbox*)self->GetComponent("Hitbox");
	if (used != NULL)
		position = used->GetPosition();
}

static bool PointInsideBox(t_Point point, t_BoundingB box)
{
	t_Point vec1 = {box.rightTop.x - box.leftTop.x, box.rightTop.y - box.leftTop.y};
	t_Point vec2 = {box.rightBottom.x - box.rightTop.x, box.rightBottom.y - box.rightTop.y};
	t_Point vec3 = {box.leftBottom.x - box.rightBottom.x, box.leftBottom.y - box.rightBottom.y};
	t_Point vec4 = {box.leftTop.x - box.leftBottom.x, box.leftTop.y - box.leftBottom.y};

	t_Point vp1 = {point.x - box.leftTop.x, point.y - box.leftTop.y};
	t_Point vp2 = {point.x - box.rightTop.x, point.y - box.rightTop.y};
	t_Point vp3 = {point.x - box.rightBottom.x, point.y - box.rightBottom.y};
	t_Point vp4 = {point.x - box.leftBottom.x, point.y - box.leftBottom.y};

	float cross1 = vec1.x * vp1.y - vec1.y * vp1.x;
	float cross2 = vec2.x * vp2.y - vec2.y * vp2.x;
	float cross3 = vec3.x * vp3.y - vec3.y * vp3.x;
	float cross4 = vec4.x * vp4.y - vec4.y * vp4.x;
	if (cross1 * cross2 >= 0.0f && cross1 * cross3 >= 0.0f && cross1 * cross4 >= 0.0f)
		return (true);
	return (false);
}

static std::optional<t_Point> FindContactPoint(t_BoundingB rect1, t_BoundingB rect2)
{
	std::vector<t_Point> contactPoints = {};
	if (PointInsideBox(rect1.leftTop, rect2)) contactPoints.emplace_back(rect1.leftTop);
	if (PointInsideBox(rect1.rightTop, rect2)) contactPoints.emplace_back(rect1.rightTop);
	if (PointInsideBox(rect1.rightBottom, rect2)) contactPoints.emplace_back(rect1.rightBottom);
	if (PointInsideBox(rect1.leftBottom, rect2)) contactPoints.emplace_back(rect1.leftBottom);

	if (PointInsideBox(rect2.leftTop, rect1)) contactPoints.emplace_back(rect2.leftTop);
	if (PointInsideBox(rect2.rightTop, rect1)) contactPoints.emplace_back(rect2.rightTop);
	if (PointInsideBox(rect2.rightBottom, rect1)) contactPoints.emplace_back(rect2.rightBottom);
	if (PointInsideBox(rect2.leftBottom, rect1)) contactPoints.emplace_back(rect2.leftBottom);

	if (contactPoints.size() == 0)
		return std::nullopt;
	float x = 0.0f;
	float y = 0.0f;
	float size = (float)contactPoints.size();
	for (t_Point p : contactPoints)
	{
		x += p.x;
		y += p.y;
	}
	t_Point ret = {x / size, y / size};
	return (ret);
}

void RigidBody::ApplyRotation(Hitbox *used, Hitbox *box, t_Point dir)
{
	if (fixed)
		return ;
	auto contact = FindContactPoint(used->GetHitBox(), box->GetHitBox());
	if (contact == std::nullopt)
		return ;
	t_Point uPosition = used->GetPosition();
	t_Point rA = {contact->x - uPosition.x, contact->y - uPosition.y};
	t_Point vecPerp = VectorPerp(rA);
	t_Point rotationalVelocity = { -rA.y * angularVelocity, rA.x * angularVelocity };
	t_Point vA = { velocity.x + rotationalVelocity.x, velocity.y + rotationalVelocity.y };
	float relVel = VectorDotProduct(vA, dir);
	if (relVel > -0.1)
		return ;

	float e = 0.3f;
	float rPerpN = VectorCross(rA, dir);
	float inverseMass = 1.0f / mass;
	t_Point dims = used->GetDimentions();
	float inertia = (1.0f / 12.0f) * mass * (dims.x * dims.x + dims.y * dims.y);
	float inverseInertia = 1.0f / inertia;
	float denom = inverseMass + (rPerpN * rPerpN) * inverseInertia;
	float j = -(1 + e) * relVel / denom;
	t_Point impulse = {dir.x * j, dir.y * j};
	velocity.x += impulse.x * inverseMass;
	velocity.y += impulse.y * inverseMass;
	angularVelocity = VectorCross(rA, impulse) * inverseInertia;
}

void RigidBody::ResolveCollisions()
{
	Hitbox *used = (Hitbox*)self->GetComponent("Hitbox");
	if (used == NULL)
		return ;
	for (int i = 0; i < RIGIDBODY_LOOPS; i++)
	{
		UpdateRigidBody(used);
		std::vector<std::tuple<Hitbox*, t_Point, float>> collisions = CheckCollisionRigidBody(used);
		if (collisions.size() == 0)
			return ;
		for (auto &[box, dir, depth] : collisions)
		{
			t_Point rect1 = box->GetPosition();
			t_Point rect2 = position;
			t_Point centerVec = {
				rect2.x - rect1.x,
				rect2.y - rect1.y
			};
			if (dir.x * centerVec.x + dir.y * centerVec.y < 0.0f)
			{
				dir.x = -dir.x;
				dir.y = -dir.y;
			}
			if (depth > 0.001f)
			{
				float alignment = velocity.x * dir.x + velocity.y * dir.y;
				if (alignment < 0.0f)
				{
					float correctionDepth = fmax(depth - 0.01f, 0.0f);
					position.x += dir.x * correctionDepth;
					position.y += dir.y * correctionDepth;
					ApplyRotation(used, box, dir);
					float vn = (velocity.x * dir.x + velocity.y * dir.y);
					velocity.x -= vn * dir.x;
					velocity.y -= vn * dir.y;
				}
			}
		}
	}
	UpdateRigidBody(used);
	if (fabs(velocity.x) < 0.001f) velocity.x = 0;
	if (fabs(velocity.y) < 0.001f) velocity.y = 0;
}

void RigidBody::Update()
{
	if (KeyHeld(SDL_SCANCODE_RIGHT))
		velocity.x = 3.0f;
	else if (KeyHeld(SDL_SCANCODE_LEFT))
		velocity.x = -3.0f;
	else
		velocity.x = 0.0f;
	if (KeyPressed(SDL_SCANCODE_SPACE))
		velocity.y = 11.0f;
}

void RigidBody::UpdateRigidBody(Hitbox *used)
{
	used->Position(position);
	used->Angle(rotation);
}

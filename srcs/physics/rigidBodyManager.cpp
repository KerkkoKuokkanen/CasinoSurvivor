
#include "rigidBodyManager.h"
#include "rigidBody.h"
#include "deltaTime.h"

std::vector<std::tuple<uint32_t, RigidBody*>> rigidBodies = {};
const float gravity = -30.0f;

void AddRigidBody(RigidBody *body)
{
	rigidBodies.push_back({body->GetKey(), body});
}

void RemoveRigidBody(RigidBody *body)
{
	uint32_t key = body->GetKey();
	for (auto &[k, b] : rigidBodies)
	{
		if (k == key)
		{
			k = 0;
			return ;
		}
	}
}

static void UpdateGravity()
{
	for (auto &[key, body] : rigidBodies)
	{
		//float acceleration = gravity + (body->force.y / body->mass);
		body->velocity.y += gravity * DeltaTime();
		body->position.y += body->velocity.y * DeltaTime();
		body->position.x += body->velocity.x * DeltaTime();
		body->rotation += body->angularVelocity * DeltaTime();
		body->angularVelocity *= 0.98f;
		body->force.y = 0;
		body->force.x = 0;
		body->ResolveCollisions();
	}
}

void UpdateRigidBody()
{
	rigidBodies.erase(std::remove_if(rigidBodies.begin(), rigidBodies.end(),
					[](std::tuple<uint32_t, RigidBody*> data) {
						return std::get<0>(data) == 0;
					}), rigidBodies.end());
	UpdateGravity();

	/*
	[Clear Forces]
		↓
	[Integrate Forces]
		↓
	[Predict Position]
		↓
	[Detect Collisions]
		↓
	[Solve Constraints]
	*/
}

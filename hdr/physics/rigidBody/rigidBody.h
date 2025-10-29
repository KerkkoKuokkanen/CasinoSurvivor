
#ifndef RIGID_BODY_H
# define RIGID_BODY_H

# include "hitbox.h"

class RigidBody : public CustomComponent
{
	private:
		uint32_t ownKey = 0;
		void UpdateRigidBody(Hitbox *used);
		void ApplyRotation(Hitbox *used, Hitbox *box, t_Point dir);
	public:
		bool fixed = false;
		t_Point position = {0.0f, 0.0f};
		t_Point velocity = {0.0f, 0.0f};
		float mass = 1.0f;
		t_Point force = {0.0f, 0.0f};

		float rotation = 0.0f;
		float angularVelocity = 0.0f;

		uint32_t GetKey() {return (ownKey);};
		void Init(void *data, size_t size) override;
		void Start() override;
		void Update() override;
		~RigidBody();
		void ResolveCollisions();
};

REGISTER_COMPONENT(RigidBody);

#endif

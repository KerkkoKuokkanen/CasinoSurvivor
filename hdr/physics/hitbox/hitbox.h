
#ifndef HIT_BOX_H
# define HIT_BOX_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "box.h"

class Hitbox : public CustomComponent
{
	private:
		t_BoundingB hitbox = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, -1.0f}, {0.0f, -1.0f}};
		t_Point pos = {0.0f, 0.0f};
		t_Point dims = {1.0f, 1.0f};
		float angle = 0.0f;
		uint32_t collisionNumber = 0;
		std::vector<uint32_t> ignored = {};
		bool apply = false;
		void SetUpOwn();
		uint32_t key = 0;
		friend std::vector<Hitbox*> CheckCollision(Hitbox *hitbox);
		friend std::vector<std::tuple<Hitbox*, t_Point, float>> CheckCollisionRigidBody(Hitbox *hitbox);
	public:
		Hitbox();
		~Hitbox();
		uint32_t GetKey() {return (key);};
		t_BoundingB &GetHitBox() {return (hitbox);};
		void Init(void *data, size_t size) override;
		void SetUpHitbox(t_Point pos, t_Point dims, float angle);

		void Angle(float angle);
		void Position(t_Point pos);
		void Dimentions(t_Point dims);

		float GetAngle() {return (angle);};
		t_Point GetPosition() {return (pos);};
		t_Point GetDimentions() {return (dims);};

		void SetCollisionNumber(uint32_t collisionNumber);
		uint32_t GetCollisionNumber() {return (collisionNumber);};

		void IgnoreCollisionNumber(uint32_t collision);
		void ClearIgnoredCollisions();

		std::vector<Hitbox*> Collision();
		void EngineUpdate() override;
		void LastUpdate() override;
};

REGISTER_COMPONENT(Hitbox);

#endif

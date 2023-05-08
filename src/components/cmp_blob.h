#pragma once
#include "cmp_physics.h"

class BlobComponent : public PhysicsComponent {
protected:
	std::weak_ptr<Entity> player;
	b2Vec2 size;
	sf::Vector2f maxVelocity;
	float groundspeed;
	bool grounded;

	bool isGrounded() const;
	void Jump();
	
public:
	void Update(double dt) override;
	explicit BlobComponent(Entity* p, const sf::Vector2f& size);
	BlobComponent() = delete;
};

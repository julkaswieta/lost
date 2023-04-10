#pragma once

#include "cmp_physics.h"

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
	b2Vec2 size;
	sf::Vector2f maxVelocity;
	bool grounded;
	bool firstJump;
	bool secondJump;
	float groundspeed;
	float timeInAir = 0.f;

	bool isGrounded() const;
	
public:
	void Update(double dt) override;

	explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);

	PlayerPhysicsComponent() = delete;
};

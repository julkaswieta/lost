#pragma once
#include "cmp_physics.h"

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
	std::shared_ptr<sf::Texture> groundLeft, groundRight, airLeft, airRight, groundTexture, airTexture;
	std::shared_ptr<Entity> sound;
	b2Vec2 size;
	sf::Vector2f maxVelocity;
	float groundspeed;
	float timeInAir = 0.f;
	bool grounded;
	bool firstJump;
	bool secondJump;
	bool jumpButtonReleased;
	bool isGrounded() const;
	
public:
	void Update(double dt) override;
	explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);
	PlayerPhysicsComponent() = delete;
};

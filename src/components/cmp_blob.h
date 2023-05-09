#pragma once
#include "cmp_physics.h"

class BlobComponent : public PhysicsComponent {
protected:
	std::shared_ptr<sf::Texture> groundLeft, groundRight, airLeft, airRight, groundTexture, airTexture;
	std::shared_ptr<Entity> sound;
	std::weak_ptr<Entity> player;
	b2Vec2 size;
	sf::Vector2f maxVelocity;
	float groundspeed;
	bool grounded;
	float timeOnGround = 0.f;

	bool isGrounded() const;
	void Jump();
	
public:
	void Update(double dt) override;
	explicit BlobComponent(Entity* p, const sf::Vector2f& size);
	BlobComponent() = delete;
};

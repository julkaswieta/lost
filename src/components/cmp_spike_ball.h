#pragma once
#include "cmp_physics.h"

class SpikeBallComponent : public PhysicsComponent {
protected:
	std::weak_ptr<Entity> player;
	b2Vec2 size;
	
public:
	void Update(double dt) override;
	explicit SpikeBallComponent(Entity* p, const sf::Vector2f& size);
	SpikeBallComponent() = delete;
};

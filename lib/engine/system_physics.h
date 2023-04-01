#pragma once

#include "Box2D/Box2D.h"
#include <SFML/System/Vector2.hpp>

namespace Physics {
	void Initialise();
	void Shutdown();
	void Update(const double&);

	std::shared_ptr<b2World> getWorld();

	const float PHYSICS_SCALE = 30.0f;
	const float PHYSICS_SCALE_INV = 1.0f / PHYSICS_SCALE;

	const sf::Vector2f boxVecToSfmlVec(const b2Vec2& in, bool scale = true);
	const b2Vec2 sfmlVecToBoxVec(const sf::Vector2f& in, bool scale = true);
	const sf::Vector2f invertHeight(const sf::Vector2f& in);
} // namespace Physics

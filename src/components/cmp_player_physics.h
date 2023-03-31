#pragma once

#include "cmp_physics.h"

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
  b2Vec2 size;
  sf::Vector2f maxVelocity;
  bool grounded;
  float groundspeed;

  bool isGrounded() const;

public:
  void Update(double dt) override;

  explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);

  PlayerPhysicsComponent() = delete;
};

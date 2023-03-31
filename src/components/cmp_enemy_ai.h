#pragma once

#include "cmp_actor_movement.h"

class EnemyAIComponent : public ActorMovementComponent {
protected:
  sf::Vector2f _direction;

public:
  void Update(double dt) override;

  explicit EnemyAIComponent(Entity* p);

  EnemyAIComponent() = delete;
};

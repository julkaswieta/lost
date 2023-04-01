#include "cmp_enemy_ai.h"

using namespace std;
using namespace sf;

void EnemyAIComponent::Update(double dt) {
  auto mov = direction * (float)(dt * speed);
  mov.x += direction.x * 16.f;
  if (!validMove(parent->getPosition() + mov)) {
    direction *= -1.f;
  }

  move(direction * (float)(dt * speed));
  ActorMovementComponent::Update(dt);
}

EnemyAIComponent::EnemyAIComponent(Entity* p) : ActorMovementComponent(p) {
  direction = Vector2f(1.0f, 0);
  speed = 100.0f;
}

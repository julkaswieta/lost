
#pragma once
#include <ecm.h>

class ActorMovementComponent : public Component {
protected:
  bool validMove(const sf::Vector2f&);
  float _speed;

public:
  void Update(double dt) override;
  void move(const sf::Vector2f&);
  void move(float x, float y);
  void Render() override {}
  explicit ActorMovementComponent(Entity* p);
  ActorMovementComponent() = delete;
};

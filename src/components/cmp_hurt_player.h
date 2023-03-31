#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class HurtComponent : public Component {
protected:
  void fire() const;
  std::weak_ptr<Entity> _player;

public:
  void Update(double dt) override;
  void Render() override {}
  explicit HurtComponent(Entity* p);
  HurtComponent() = delete;
};

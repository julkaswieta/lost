#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class BulletComponent : public Component {
protected:
  float lifetime;

public:
  void Update(double dt) override;
  void Render() override {}
  explicit BulletComponent(Entity* p, float lifetime = 3.f);
  BulletComponent() = delete;
};

#pragma once

#include "engine.h"

class Level1Scene : public Scene {
public:
  void Load() override;

  void Unload() override;

  void Update(const double& dt) override;

  void Render() override;
};

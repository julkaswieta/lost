#pragma once

#include "engine.h"

class Level2Scene : public Scene {
public:
  void Load() override;

  void Unload() override;

  void Update(const double& dt) override;

  void Render() override;
};
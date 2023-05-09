/**
* scene_level_menu.h: header file for LevelMenuScene class
*
* Author: Dillon Aitken
* 
* Last modified: 09/05/2023
*/
#pragma once
#include "engine.h"

/// Scene for first level
class LevelMenuScene : public Scene {
public:
  void Load() override;
  void Unload() override;
  void Update(const double& dt) override;
  void Render() override;

};

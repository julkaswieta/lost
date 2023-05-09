/**
* Main game header file
* 
* Author: "Lost" devs
* 
* Last modified: 09/05/2023
*/
#pragma once
#include "SFML/Window/Keyboard.hpp"
#include "scenes/scene_main_menu.h"
#include "scenes/scene_level_menu.h"
#include "scenes/scene_level_1.h"
#include "scenes/scene_level_2.h"
#include "scenes/scene_level_3.h"
#include "scenes/scene_settings.h"
#include "scenes/scene_controls.h"
#include "scenes/scene_volume.h"
#include "scenes/scene_resolution.h"
#include "scenes/scene_end.h"
#include "scenes/scene_end_level.h"

extern MainMenuScene menu;
extern LevelMenuScene levelMenu;
extern Level1Scene level1;
extern Level2Scene level2;
extern Level3Scene level3;
extern SettingsScene settings;
extern ControlsScene controls;
extern VolumeScene volume;
extern ResolutionScene resolution;
extern EndScene endScene;
extern EndLevelScene endLevel;

extern std::vector<Scene*> levels;
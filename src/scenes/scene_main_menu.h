/**
* scene_main_menu.h: header file for MainMenuScene class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#pragma once

#include "engine.h"
#include "scene_menu.h"

/// Main menu scene
class MainMenuScene : public MenuScene {
public:
	MainMenuScene() = default;
	~MainMenuScene() override = default;
	void Load() override;

private:
	void executeSelectedOption() override;
};

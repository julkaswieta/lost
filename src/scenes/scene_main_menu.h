#pragma once

#include "engine.h"
#include "scene_menu.h"

class MainMenuScene : public MenuScene {
public:
	MainMenuScene() = default;
	~MainMenuScene() override = default;
	void Load() override;

private:
	void executeSelectedOption() override;
};

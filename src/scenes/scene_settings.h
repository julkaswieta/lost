#pragma once
#include "scene_menu.h"

class SettingsScene : public MenuScene {
public:
	SettingsScene() = default;
	~SettingsScene() override = default;
	void Load() override;

private:
	void executeSelectedOption();
};
/**
* scene_settings.h: header file for SettingsScene class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#pragma once
#include "scene_menu.h"

/// A scene for accessing specific settings
class SettingsScene : public MenuScene {
public:
	SettingsScene() = default;
	~SettingsScene() override = default;
	void Load() override;

private:
	void executeSelectedOption();
};
/**
* scene_end_level.h: header file for EndLevelScene class
*
* Author: Julia Swietochowska
* Last modified: 08/05/2023
*/
#pragma once
#include "scene_menu.h"

/// A scene displayed at the end of the last level
class EndLevelScene : public MenuScene {
public:
	EndLevelScene() = default;
	~EndLevelScene() override = default;
	void Load() override;
	void Unload() override;
	void Update(const double& dt) override;

private:
	int previousLevel;
	int deathCount;
	float time;
	int stars;
	void executeSelectedOption() override;
};


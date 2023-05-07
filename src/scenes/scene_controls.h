/**
* scene_controls.h: header file for ControlsScene class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#pragma once
#include "scene_menu.h"

/// A scene for controls settings
/// Allows remapping certain controls and displays all navigation controls
class ControlsScene : public MenuScene {
public:
	ControlsScene() = default;
	~ControlsScene() override = default;
	void Load() override;
	void Update(const double& dt) override;

private:
	const int SPACING = 50;
	const int TOP_MARGIN = 50;
	const int LEFT_MARGIN = 200;

	void moveUp() override;
	void moveDown() override;
	void executeSelectedOption() override;
	void updateControlsUI();
	void restoreDefaults();
	sf::Vector2f positionOptionInWindow(int i);
};
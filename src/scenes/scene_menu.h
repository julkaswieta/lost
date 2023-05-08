/**
* scene_menu.h: header file for MenuScene class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#pragma once
#include "scene.h"

/// An abstract class providing a template for menu scenes
class MenuScene : public Scene {
public:
	MenuScene() = default;
	~MenuScene() override = default;
	virtual void Load() override = 0;
	void Unload() override;
	void Update(const double& dt) override;

protected:
	const int TOP_MARGIN = 100;

	static std::vector<std::shared_ptr<Entity>> options;
	int selectedOptionIndex = -1;

	virtual void moveUp();
	virtual void moveDown();

	virtual void executeSelectedOption() = 0;
};


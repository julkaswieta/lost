/**
* scene_level1.h: header file for Level1Scene class
*
* Author: Dillon Aitken
* Pause Menu: Julia Swietochowska
* Last modified: 04/05/2023
*/
#pragma once

#include "engine.h"

/// Scene for first level
class Level1Scene : public Scene {
public:
  void Load() override;
  void Unload() override;
  void Update(const double& dt) override;
  void Render() override;
  void AddCollected(std::string tag);

private:
	static std::vector<std::shared_ptr<Entity>> menuOptions;
	int selectedOptionIndex = -1;
	std::vector<std::string> collected;

	void loadPauseMenu();
	void moveUp();
	void moveDown();
	void executeSelectedOption();
	void displayMenu();
	void hideMenu();
};

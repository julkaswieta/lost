#pragma once

#include "engine.h"

class Level1Scene : public Scene {
public:
  void Load() override;

  void Unload() override;

  void Update(const double& dt) override;

  void Render() override;

private:
	static std::vector<std::shared_ptr<Entity>> menuOptions;
	int selectedOptionIndex = -1;

	void loadPauseMenu();
	void moveUp();
	void moveDown();
	void executeSelectedOption();
	void displayMenu();
	void hideMenu();
};

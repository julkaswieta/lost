#pragma once

#include "engine.h"

class Level1Scene : public Scene {
public:
  void Load() override;

  void Unload() override;

  void Update(const double& dt) override;

  void Render() override;

private:
	std::vector<std::shared_ptr<Entity>> menuOptions;
	int selectedOptionIndex;

	void moveUp();
	void moveDown();
	void executeSelectedOption();
	void displayMenu();
	void hideMenu();
};

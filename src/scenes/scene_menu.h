#pragma once

#include "engine.h"

class MenuScene : public Scene {
public:
	MenuScene() = default;
	~MenuScene() override = default;
	void Load() override;

	void Update(const double& dt) override;

private:
	const int OPTIONS_COUNT = 3;
	const int TOP_MARGIN = 100;

	std::vector<std::shared_ptr<Entity>> options;
	int selectedOptionIndex;

	void moveUp();
	void moveDown();

	void executeSelectedOption();
};

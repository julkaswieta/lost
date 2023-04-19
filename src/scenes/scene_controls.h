#pragma once
#include "scene.h"

class ControlsScene : public Scene {
public:
	ControlsScene() = default;
	~ControlsScene() override = default;
	void Load() override;
	void Unload() override;
	void Update(const double& dt) override;

private:
	const int ACTIVE_OPTIONS_COUNT = 7;
	const int SPACING = 50;
	const int TOP_MARGIN = 50;
	const int LEFT_MARGIN = 200;

	std::vector<std::shared_ptr<Entity>> options;
	int selectedOptionIndex;

	void moveUp();
	void moveDown();
	void executeSelectedOption();
	void updateControlsUI();

	sf::Vector2f positionOptionInWindow(int i);
};
#pragma once
#include "scene.h"

class SettingsScene : public Scene {
public:
	SettingsScene() = default;
	~SettingsScene() override = default;
	void Load() override;
	void Unload() override;
	void Update(const double& dt) override;

private:
	const int OPTIONS_COUNT = 4;
	const int TOP_MARGIN = 100;

	std::vector<std::shared_ptr<Entity>> options;
	int selectedOptionIndex;

	void moveUp();
	void moveDown();

	void executeSelectedOption();
};
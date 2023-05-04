#pragma once
#include "scene.h"

class MenuScene : public Scene {
public:
	MenuScene() = default;
	~MenuScene() override = default;
	virtual void Load() override = 0;
	void Unload() override;
	void Update(const double& dt) override;

protected:
	const int TOP_MARGIN = 100;

	std::vector<std::shared_ptr<Entity>> options;
	int selectedOptionIndex;

	virtual void moveUp();
	virtual void moveDown();

	virtual void executeSelectedOption() = 0;
};


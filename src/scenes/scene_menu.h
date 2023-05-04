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

	static std::vector<std::shared_ptr<Entity>> options;
	int selectedOptionIndex = -1;

	virtual void moveUp();
	virtual void moveDown();

	virtual void executeSelectedOption() = 0;
};


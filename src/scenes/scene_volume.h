#pragma once
#include "scene.h"

class VolumeScene : public Scene {
public:
	VolumeScene() = default;
	~VolumeScene() override = default;
	void Load() override;
	void Unload() override;
	void Update(const double& dt) override;

private:
	const int OPTIONS_COUNT = 2;
	const int TOP_MARGIN = 100;
	bool volumeChangeActive;
	int volume;

	std::vector<std::shared_ptr<Entity>> options;
	int selectedOptionIndex;

	void moveUp();
	void moveDown();
	void volumeUp();
	void volumeDown();

	void executeSelectedOption();
};
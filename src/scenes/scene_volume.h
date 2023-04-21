#pragma once
#include "scene_menu.h"

class VolumeScene : public MenuScene {
public:
	VolumeScene() = default;
	~VolumeScene() override = default;
	void Load() override;
	void Update(const double& dt) override;

private:
	bool volumeChangeActive;
	int volume;

	void moveUp() override;
	void moveDown() override;
	void volumeUp();
	void volumeDown();

	void executeSelectedOption() override ;
};
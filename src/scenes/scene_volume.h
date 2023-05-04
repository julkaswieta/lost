#pragma once
#include "scene_menu.h"

class VolumeScene : public MenuScene {
public:
	VolumeScene() = default;
	~VolumeScene() override = default;
	void Load() override;
	void Update(const double& dt) override;

private:
	bool volumeChangeActive = false;
	int volume = 50;

	void moveUp() override;
	void moveDown() override;
	void volumeUp();
	void volumeDown();
	void resetFormatting();
	void executeSelectedOption() override ;
};
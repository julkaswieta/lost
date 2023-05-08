/**
* scene_volume.h: header file for VolumeScene class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/#pragma once
#include "scene_menu.h"

/// A scene for changing volume setting
class VolumeScene : public MenuScene {
public:
	VolumeScene() = default;
	~VolumeScene() override = default;
	void Load() override;
	void Update(const double& dt) override;

private:
	bool volumeChangeActive = false;
	int localVolume = 50;

	void moveUp() override;
	void moveDown() override;
	void volumeUp();
	void volumeDown();
	void resetFormatting();
	void executeSelectedOption() override ;
};
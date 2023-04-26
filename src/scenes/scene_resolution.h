#pragma once
#include "scene_menu.h"

class ResolutionScene : public MenuScene {
public:
	ResolutionScene() = default;
	~ResolutionScene() override = default;
	void Load() override;
	void Update(const double& dt) override;

private:
	bool resolutionChangeActive;
	bool windowModeChangeActive;
	sf::Vector2f resolution;
	std::string currentResolutionOption;
	int currentWindowMode; // 0 for windowed, 1 for fullscreen
	std::vector<std::shared_ptr<Entity>> resolutions;
	std::vector<std::shared_ptr<Entity>> windowModes;

	void moveUp() override;
	void moveDown() override;
	void nextResolution();
	sf::Vector2f positionElement(int elemIndex);
	void executeSelectedOption() override;
	sf::Vector2f resolutionToVector(std::string resolutionText);
	void displayCurrentSettings();
};
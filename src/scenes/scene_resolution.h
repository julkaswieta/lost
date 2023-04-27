#pragma once
#include "scene_menu.h"

class ResolutionScene : public MenuScene {
public:
	ResolutionScene() = default;
	~ResolutionScene() override = default;
	void Load() override;
	void Update(const double& dt) override;
	void Unload() override;

private:
	bool resolutionChangeActive;
	bool windowModeChangeActive;

	sf::Vector2u resolution;
	std::string currentResolutionOption;
	int resolutionCounter;
	int currentWindowMode = -1; // 0 for fullscreen, 1 for windowed

	std::vector<std::shared_ptr<Entity>> resolutions;
	std::vector<std::shared_ptr<Entity>> windowModes;

	void moveUp() override;
	void moveDown() override;
	void nextResolution(bool moveUp);
	void nextWindowMode(bool moveUp);
	void executeSelectedOption() override;

	// helper functions
	sf::Vector2f positionElement(int elemIndex);
	sf::Vector2u resolutionToVector(std::string resolutionText);
	void displayCurrentSettings();
	void resetFormatting();
	void loadOptions();
	void loadResolutions();
	void loadWindowModes();
	void changeSettings();
	void updateElementsPosition();
};
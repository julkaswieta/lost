/**
* scene_resolution.h: header file for ResolutionScene class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#pragma once
#include "scene_menu.h"

/// Scene for resolution settings 
/// Allows changing resolution and window mode
class ResolutionScene : public MenuScene {
public:
	ResolutionScene() = default;
	~ResolutionScene() override = default;
	void Load() override;
	void Update(const double& dt) override;
	void Unload() override;

	static sf::Vector2u getResolution(int index);

private:
	bool resolutionChangeActive = false;
	bool windowModeChangeActive = false;

	sf::Vector2u resolution = sf::Vector2u(0,0);
	int resolutionCounter = -1;
	int currentWindowMode = -1; // 0 for fullscreen, 1 for windowed

	static std::vector<std::shared_ptr<Entity>> resolutions;
	static std::vector<std::shared_ptr<Entity>> windowModes;

	void moveUp() override;
	void moveDown() override;
	void nextResolution(bool moveUp);
	void nextWindowMode(bool moveUp);
	void executeSelectedOption() override;

	// helper functions
	sf::Vector2f positionElement(int elemIndex);
	static sf::Vector2u resolutionToVector(std::string resolutionText);
	void displayCurrentSettings();
	void resetFormatting();
	void loadOptions();
	void loadResolutions();
	void loadWindowModes();
	void changeSettings();
	void updateElementsPosition();
};
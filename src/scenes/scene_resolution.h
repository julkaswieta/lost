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
	sf::Vector2f resolution;
	std::vector<std::shared_ptr<Entity>> resolutions;
	std::vector<std::shared_ptr<Entity>> windowModes;

	void moveUp() override;
	void moveDown() override;
	void nextResolution();
	sf::Vector2f positionElement(int elemIndex);

	void executeSelectedOption() override;
};
#pragma once

#include "engine.h"

class MainMenuScene : public Scene {
public:
	void Load() override;

	void Unload() override;

	void Update(const double& dt) override;

	void Render() override;
};
#pragma once
#include "scene.h"

class ControlsScene : public Scene {
public:
	ControlsScene() = default;
	~ControlsScene() override = default;
	void Load() override;
	void Unload() override;
	void Update(const double& dt) override;
};
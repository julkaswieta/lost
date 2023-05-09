/**
* scene_end.h: header file for EndScene class
*
* Author: Julia Swietochowska
* Last modified: 08/05/2023
*/
#pragma once
#include "scene.h"

/// A scene displayed at the end of the last level
class EndScene : public Scene {
public:
	EndScene() = default;
	~EndScene() override = default;
	void Load() override;
	void Update(const double& dt) override;
};


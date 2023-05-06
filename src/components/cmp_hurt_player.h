#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class HurtComponent : public Component {
protected:
	std::weak_ptr<Entity> player;
	float size;

public:
	void Update(double dt) override;
	void Render() override {}
	explicit HurtComponent(Entity* p, float s);
	HurtComponent() = delete;
};

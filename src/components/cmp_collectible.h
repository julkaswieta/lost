#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class CollectibleComponent : public Component {
protected:
	std::shared_ptr<Entity> sound;
	std::weak_ptr<Entity> player;
	float size;

public:
	void Update(double dt) override;
	void Render() override {}
	explicit CollectibleComponent(Entity* p, float s);
	CollectibleComponent() = delete;
};

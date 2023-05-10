#include "../controls.h"
#include "engine.h"
#include "system_physics.h"
#include "cmp_spike_ball.h"

#include <SFML/Window/Keyboard.hpp>
#include <LevelSystem.h>

using namespace std;
using namespace sf;
using namespace Physics;

void SpikeBallComponent::Update(double dt) {
    const auto pos = parent->getPosition();

    if (auto pl = player.lock()) {
        if (pl->getPosition().y > parent->getPosition().y) {
            if ((pl->getPosition().x > parent->getPosition().x - 60.f) &&
                (pl->getPosition().x < parent->getPosition().x + 60.f)) {
                body->SetActive(true);
                impulse(Vector2f(0.f, 0.05f));
            }
        }
    }

    PhysicsComponent::Update(dt);
}

SpikeBallComponent::SpikeBallComponent(Entity* p, const Vector2f& s)
    : PhysicsComponent(p, true, s), player(parent->scene->ents.find("player")[0]) {
    size = sfmlVecToBoxVec(s, true);
    body->SetActive(false);
    body->SetSleepingAllowed(false);
    body->SetFixedRotation(true);
    body->SetBullet(true);
}
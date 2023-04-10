#include "cmp_hurt_player.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HurtComponent::Update(double dt) {
    if (auto pl = player.lock()) {
        if (length(pl->getPosition() - parent->getPosition()) < 25.0) {
            pl->setForDelete();
            parent->setForDelete();
        }
    }
}

HurtComponent::HurtComponent(Entity* p)
    : Component(p), player(parent->scene->ents.find("player")[0]) {}

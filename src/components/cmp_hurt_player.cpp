#include "cmp_hurt_player.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HurtComponent::Update(double dt) {
    if (auto pl = player.lock()) {
        if (length(pl->getPosition() - parent->getPosition()) < size) {
            pl->setForDelete();
        }
    }
}

HurtComponent::HurtComponent(Entity* p, float s)
    : Component(p), player(parent->scene->ents.find("player")[0]), size(s) {}

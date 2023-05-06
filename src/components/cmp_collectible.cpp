#include "cmp_collectible.h"
#include <engine.h>

using namespace std;
using namespace sf;

void CollectibleComponent::Update(double dt) {
    if (auto pl = player.lock()) {
        if (length(pl->getPosition() - parent->getPosition()) < size) {
            pl->setForDelete();
            parent->setForDelete();
        }
    }
}

CollectibleComponent::CollectibleComponent(Entity* p, float s = 55.f)
    : Component(p), player(parent->scene->ents.find("player")[0]), size(s) {}

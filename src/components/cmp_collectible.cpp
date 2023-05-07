#include "cmp_collectible.h"
#include "../scenes/scene_level1.h"
#include <engine.h>

using namespace std;
using namespace sf;

void CollectibleComponent::Update(double dt) {
    if (auto pl = player.lock()) {
        if (length(pl->getPosition() - parent->getPosition()) < size) {
            auto level = reinterpret_cast<Level1Scene*>(parent->scene);
            string tag = parent->getTags().begin()->data();
            level->AddCollected(tag);
            parent->setForDelete();
        }
    }
}

CollectibleComponent::CollectibleComponent(Entity* p, float s)
    : Component(p), player(parent->scene->ents.find("player")[0]), size(s) {}

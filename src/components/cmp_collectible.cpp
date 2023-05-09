#include "cmp_collectible.h"

#include <engine.h>

#include "../scenes/scene_level_1.h"
#include "cmp_game_sounds.h"

using namespace std;
using namespace sf;

void CollectibleComponent::Update(double dt) {
    if (auto pl = player.lock()) {
        if (length(pl->getPosition() - parent->getPosition()) < size) {
            sound->getComponents<GameSoundsComponent>()[0]->playCollectSound();
            auto level = reinterpret_cast<Level1Scene*>(parent->scene);
            string tag = parent->getTags().begin()->data();
            level->AddCollected(tag);
            parent->setForDelete();
        }
    }
}

CollectibleComponent::CollectibleComponent(Entity* p, float s) : Component(p),
    player(parent->scene->ents.find("player")[0]), 
    sound(parent->scene->ents.find("gameSounds")[0]),
    size(s) {}

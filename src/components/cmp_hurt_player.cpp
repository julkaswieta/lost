#include "engine.h"
#include "cmp_game_sounds.h"
#include "cmp_hurt_player.h"

using namespace std;

void HurtComponent::Update(double dt) {
    if (auto pl = player.lock()) {
        if (length(pl->getPosition() - parent->getPosition()) < size) {
            pl->setForDelete();
        }
    }
}

HurtComponent::HurtComponent(Entity* p, float s) : Component(p), 
    player(parent->scene->ents.find("player")[0]),
    sound(parent->scene->ents.find("gameSounds")[0]),
    size(s) {}

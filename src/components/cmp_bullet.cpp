#include "cmp_bullet.h"
using namespace std;
using namespace sf;

void BulletComponent::Update(double dt) {
    lifetime -= dt;
    if (lifetime <= 0.f) {
        parent->setForDelete();
    }
}

BulletComponent::BulletComponent(Entity* p, float lifetime)
    : Component(p), lifetime(lifetime) {}

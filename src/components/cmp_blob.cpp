#include "../controls.h"
#include "engine.h"
#include "system_physics.h"
#include "cmp_blob.h"

#include <SFML/Window/Keyboard.hpp>
#include <LevelSystem.h>

using namespace std;
using namespace sf;
using namespace Physics;

bool BlobComponent::isGrounded() const {
    auto touch = getTouching();
    const auto& pos = body->GetPosition();
    const float halfPlrHeigt = size.y * .5f;
    const float halfPlrWidth = size.x * .52f;
    b2WorldManifold manifold;
    for (const auto& contact : touch) {
        contact->GetWorldManifold(&manifold);
        const int numPoints = contact->GetManifold()->pointCount;
        bool onTop = numPoints > 0;
        // If all contacts are below the player.
        for (int j = 0; j < numPoints; j++) {
            onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
        }
        if (onTop) {
            return true;
        }
    }

    return false;
}

void BlobComponent::Update(double dt) {
    const auto pos = parent->getPosition();

    //Teleport to start if we fall off map.
    if (pos.y > ls::getHeight() * ls::getTileSize()) {
        teleport(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]));
    }

    if (auto pl = player.lock()) {
        if (pl->getPosition().x > parent->getPosition().x) {
            if (getVelocity().x < maxVelocity.x)
                impulse({ (float)(dt * groundspeed), 0 });
        }
        else {
            if (getVelocity().x > -maxVelocity.x)
                impulse({ -(float)(dt * groundspeed), 0 });
        }
    }
    
    // Dampen X axis movement
    // dampen({ 0.9f, 1.0f });
    
    // Check to see if we have landed yet
    grounded = isGrounded();

    if (!grounded) {
        groundspeed = 10.f;
        setFriction(0.f);
    }
    else {
        Jump();
        groundspeed = 15.f;
        setFriction(0.2f);
    }

    // Clamp velocity.
    auto v = getVelocity();
    v.x = copysign(min(abs(v.x), maxVelocity.x), v.x);
    v.y = copysign(min(abs(v.y), maxVelocity.y), v.y);
    setVelocity(v);

    PhysicsComponent::Update(dt);
}

void BlobComponent::Jump() {
    grounded = isGrounded();

    if (grounded) {
        const auto pos = parent->getPosition();
        setVelocity(Vector2f(getVelocity().x, 0.f));
        teleport(Vector2f(pos.x, pos.y - 2.0f));
        impulse(Vector2f(0, -10.f));
    }
}

BlobComponent::BlobComponent(Entity* p, const Vector2f& s)
    : PhysicsComponent(p, true, s), player(parent->scene->ents.find("player")[0]) {
    size = sfmlVecToBoxVec(s, true);
    maxVelocity = Vector2f(100.f, 300.f);
    groundspeed = 30.f;
    grounded = false;
    body->SetSleepingAllowed(false);
    body->SetFixedRotation(true);
    //Bullet items have higher-res collision detection
    body->SetBullet(true);
}
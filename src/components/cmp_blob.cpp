#include "../controls.h"
#include "engine.h"
#include "system_physics.h"
#include "system_resources.h"
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
            parent->getComponents<SpriteComponent>()[0]->setTexure(groundTexture);
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
            if (getVelocity().x < maxVelocity.x) {
                impulse({ (float)(dt * groundspeed), 0 });
                parent->getComponents<SpriteComponent>()[0]->getSprite().setScale(Vector2f(-1.f, 1.f));
            }
        }
        else {
            if (getVelocity().x > -maxVelocity.x) {
                impulse({ -(float)(dt * groundspeed), 0 });
                parent->getComponents<SpriteComponent>()[0]->getSprite().setScale(Vector2f(1.f, 1.f));
            }
        }
    }
    
    // Dampen X axis movement
    // dampen({ 0.9f, 1.0f });
    
    // Check to see if we have landed yet
    grounded = isGrounded();

    if (!grounded) {
        groundspeed = 10.f;
        setFriction(0.f);
        timeOnGround = 0.f;
    }
    else {
        groundspeed = 15.f;
        setFriction(0.2f);
        timeOnGround += dt;
        if (timeOnGround > 0.3f)
            Jump();
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
        parent->getComponents<SpriteComponent>()[0]->setTexure(airTexture);
        const auto pos = parent->getPosition();
        setVelocity(Vector2f(getVelocity().x, 0.f));
        teleport(Vector2f(pos.x, pos.y - 2.0f));
        impulse(Vector2f(0, -10.f));
    }
}

BlobComponent::BlobComponent(Entity* p, const Vector2f& s)
    : PhysicsComponent(p, true, s), player(parent->scene->ents.find("player")[0]) {
    size = sfmlVecToBoxVec(s, true);

    // Load textures
    groundTexture = Resources::get<Texture>("BlobGround.png");
    airTexture = Resources::get<Texture>("BlobAir.png");
    parent->getComponents<SpriteComponent>()[0]->getSprite().setOrigin(Vector2f(25.f, 0.f));

    maxVelocity = Vector2f(100.f, 300.f);
    timeOnGround = 0.f;
    groundspeed = 30.f;
    grounded = false;
    body->SetSleepingAllowed(false);
    body->SetFixedRotation(true);
    body->SetBullet(true);
}
#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include "../controls.h"

using namespace std;
using namespace sf;
using namespace Physics;

bool PlayerPhysicsComponent::isGrounded() const {
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

void PlayerPhysicsComponent::Update(double dt) {
    const auto pos = parent->getPosition();

    //Teleport to start if we fall off map.
    if (pos.y > ls::getHeight() * ls::getTileSize()) {
        teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    }

    if (Keyboard::isKeyPressed(Controls::MoveLeft) ||
        Keyboard::isKeyPressed(Controls::MoveRight)) {
        // Moving Either Left or Right
        if (Keyboard::isKeyPressed(Controls::MoveRight)) {
            if (getVelocity().x < maxVelocity.x)
                impulse({ (float)(dt * groundspeed), 0 });
        }
        else {
            if (getVelocity().x > -maxVelocity.x)
                impulse({ -(float)(dt * groundspeed), 0 });
        }
    }
    else {
        // Dampen X axis movement
        dampen({ 0.9f, 1.0f });
    }

    // Handle Jump
    if (Keyboard::isKeyPressed(Controls::Jump)) {
        grounded = isGrounded();

        if (grounded) {
            setVelocity(Vector2f(getVelocity().x, 0.f));
            teleport(Vector2f(pos.x, pos.y - 2.0f));
            impulse(Vector2f(0, -16.f));
            timeInAir = 0.f; // Reset the time in air when the player jumps off the ground
            cout << "Jumped once!\n";
            jumpButtonReleased = false;
        }
        else {
            if (firstJump && !secondJump && jumpButtonReleased) {

                setVelocity(Vector2f(getVelocity().x, 0.f));
                impulse(Vector2f(0, -16.f));
                cout << "Jumped twice!\n";
                firstJump = false;
                secondJump = true;
            }
        }
    }
    else {
        jumpButtonReleased = true;
    }

    // Check to see if we have landed yet
    grounded = isGrounded();

    // In the Update function, increment the timeInAir if the player is in the air
    if (!grounded) {
        setFriction(0.f);
        timeInAir += dt;
        if (!firstJump && !secondJump && timeInAir > 0.2f) {
            firstJump = true;
        }
    }
    else {
        setFriction(0.1f);
        timeInAir = 0.f;
        firstJump = false;
        secondJump = false;
    }

    // Clamp velocity.
    auto v = getVelocity();
    v.x = copysign(min(abs(v.x), maxVelocity.x), v.x);
    v.y = copysign(min(abs(v.y), maxVelocity.y), v.y);
    setVelocity(v);

    PhysicsComponent::Update(dt);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p, const Vector2f& s)
    : PhysicsComponent(p, true, s) {
    size = sfmlVecToBoxVec(s, true);
    maxVelocity = Vector2f(200.f, 400.f);
    groundspeed = 30.f;
    timeInAir = 0.f;
    grounded = false;
    firstJump = false;
    secondJump = false;
    body->SetSleepingAllowed(false);
    body->SetFixedRotation(true);
    //Bullet items have higher-res collision detection
    body->SetBullet(true);
}
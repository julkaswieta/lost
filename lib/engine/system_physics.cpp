#include "system_physics.h"
#include "Box2D/Box2D.h"

using namespace std;
using namespace sf;

namespace Physics {
    static shared_ptr<b2World> world;
    const int32 velocityIterations = 6;
    const int32 positionIterations = 2;

    // Construct a world object, which will hold and simulate the rigid bodies
    void Initialise() {
        b2Vec2 gravity(0.0f, -10.0f);
        world.reset(new b2World(gravity));
    }

    // Steps all bodies in the world
    void Update(const double& dt) {
        world->Step((float)dt, velocityIterations, positionIterations);
    }

    // Clears and resets the world
    void Shutdown() {
        world.reset();
    }

    // returns the world object
    shared_ptr<b2World> getWorld() { return world; }

    // converts a box2d b2Vec2 to sfml Vector2
    const Vector2f boxVecToSfmlVec(const b2Vec2& in, bool scale) {
        if (scale)
            return Vector2f((in.x * PHYSICS_SCALE), (in.y * PHYSICS_SCALE));
        else
            return Vector2f(in.x, in.y);
    }

    // converts an sfml Vector2 to box2d b2Vec2
    const b2Vec2 sfmlVecToBoxVec(const Vector2f& in, bool scale) {
        if (scale) 
            return b2Vec2((in.x * PHYSICS_SCALE_INV), (in.y * PHYSICS_SCALE_INV));
        else 
            return b2Vec2(in.x, in.y);
    }

    // inverts the height in a vector (height increases from bottom of the screen 
    // towards x-axis in box2d and the other way round in sfml)
    const Vector2f invertHeight(const Vector2f& in) {
        return Vector2f(in.x, 720 - in.y);
    }
} // namespace Physics
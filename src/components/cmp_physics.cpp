#include "cmp_physics.h"
#include "system_physics.h"

using namespace std;
using namespace sf;

using namespace Physics;

void PhysicsComponent::Update(double dt) {
    parent->setPosition(invertHeight(boxVecToSfmlVec(body->GetPosition())));
    parent->setRotation((180 / b2_pi) * body->GetAngle());
}

PhysicsComponent::PhysicsComponent(Entity* p, bool dyn, const Vector2f& size)
    : Component(p), dynamic(dyn) {

    b2BodyDef BodyDef;
    // Is Dynamic(moving), or static(Stationary)
    BodyDef.type = dynamic ? b2_dynamicBody : b2_staticBody;
    BodyDef.position = sfmlVecToBoxVec(invertHeight(p->getPosition()));

    // Create the body
    body = Physics::getWorld()->CreateBody(&BodyDef);
    body->SetActive(true);
    {
        // Create the fixture shape
        b2PolygonShape Shape;
        // SetAsBox box takes HALF-Widths!
        Shape.SetAsBox(sfmlVecToBoxVec(size).x * 0.5f, sfmlVecToBoxVec(size).y * 0.5f);
        b2FixtureDef FixtureDef;
        // Fixture properties
        // FixtureDef.density = _dynamic ? 10.f : 0.f;
        FixtureDef.friction = dynamic ? 0.1f : 0.8f;
        FixtureDef.restitution = .2;
        FixtureDef.shape = &Shape;
        // Add to body
        fixture = body->CreateFixture(&FixtureDef);
        //_fixture->SetRestitution(.9)
        FixtureDef.restitution = .2;
    }

    // An ideal pod/capsule shape should be used for the player,
    // this isn't built into B2d, but we can combine two shapes to do so.
    // This would allow the player to go up steps
    /*
    BodyDef.bullet = true;
    b2PolygonShape shape1;
    shape1.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
    {
        b2PolygonShape poly;
        poly.SetAsBox(0.45f, 1.4f);
        b2FixtureDef FixtureDefPoly;

        FixtureDefPoly.shape = &poly;
        body->CreateFixture(&FixtureDefPoly);
    }
    {
        b2CircleShape circle;
        circle.m_radius = 0.45f;
        circle.m_p.Set(0, -1.4f);
        b2FixtureDef FixtureDefCircle;
        FixtureDefCircle.shape = &circle;
        body->CreateFixture(&FixtureDefCircle);
    }
    */
}

void PhysicsComponent::setFriction(float r) { fixture->SetFriction(r); }

void PhysicsComponent::setMass(float m) { fixture->SetDensity(m); }

void PhysicsComponent::teleport(const sf::Vector2f& v) {
    body->SetTransform(sfmlVecToBoxVec(invertHeight(v)), 0.0f);
}

const sf::Vector2f PhysicsComponent::getVelocity() const {
    return boxVecToSfmlVec(body->GetLinearVelocity(), true);
}
void PhysicsComponent::setVelocity(const sf::Vector2f& v) {
    body->SetLinearVelocity(sfmlVecToBoxVec(v, true));
}

b2Fixture* const PhysicsComponent::getFixture() const { return fixture; }

PhysicsComponent::~PhysicsComponent() {
    auto a = Physics::getWorld();
    body->SetActive(false);
    Physics::getWorld()->DestroyBody(body);
    // delete body;
    body = nullptr;
}

void PhysicsComponent::Render() {}

void PhysicsComponent::impulse(const sf::Vector2f& i) {
    auto a = b2Vec2(i.x, i.y * -1.0f);
    body->ApplyLinearImpulseToCenter(a, true);
}

void PhysicsComponent::dampen(const sf::Vector2f& i) {
    auto vel = body->GetLinearVelocity();
    vel.x *= i.x;
    vel.y *= i.y;
    body->SetLinearVelocity(vel);
}

bool PhysicsComponent::isTouching(const PhysicsComponent& pc) const {
    b2Contact* bc;
    return isTouching(pc, bc);
}

bool PhysicsComponent::isTouching(const PhysicsComponent& pc, b2Contact const* bc) const {
    const auto otherFixture = pc.getFixture();
    const auto& w = *Physics::getWorld();
    const auto contactList = w.GetContactList();
    const auto clc = w.GetContactCount();

    for (int32 i = 0; i < clc; i++) {
        const auto& contact = (contactList[i]);
        if (contact.IsTouching() && ((contact.GetFixtureA() == fixture &&
                                      contact.GetFixtureA() == otherFixture) ||
                                     (contact.GetFixtureA() == otherFixture &&
                                      contact.GetFixtureA() == fixture))) {
            bc = &contact;
            return true;
        }
    }

    return false;
}

std::vector<const b2Contact const*> PhysicsComponent::getTouching() const {
    std::vector<const b2Contact const*> ret;

    b2ContactEdge* edge = body->GetContactList();
    while (edge != NULL) {
        const b2Contact* contact = edge->contact;
        if (contact->IsTouching()) {
            ret.push_back(contact);
        }
        edge = edge->next;
    }

    return ret;
}

void PhysicsComponent::setRestitution(float r) {
    fixture->SetRestitution(r);
}

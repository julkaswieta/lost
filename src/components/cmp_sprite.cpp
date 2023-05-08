#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace std;

void SpriteComponent::setTexure(std::shared_ptr<sf::Texture> tex) {
    texture = tex;
    sprite->setTexture(*texture);
}

SpriteComponent::SpriteComponent(Entity* p, sf::Vector2f s)
    : Component(p), sprite(make_shared<sf::Sprite>()), size(s) {}

void SpriteComponent::Update(double dt) {
    sprite->setPosition(parent->getPosition() - sf::Vector2f(size.x / 2.f, size.y / 2.f));
    sprite->setRotation(sf::degrees(parent->getRotation()));
}

void SpriteComponent::Render() { Renderer::Queue(sprite.get()); }

void ShapeComponent::Update(double dt) {
    shape->setPosition(parent->getPosition());
    shape->setRotation(sf::degrees(parent->getRotation()));
}

void ShapeComponent::Render() { Renderer::Queue(shape.get()); }

sf::Shape& ShapeComponent::getShape() const { return *shape; }

ShapeComponent::ShapeComponent(Entity* p)
    : Component(p), shape(make_shared<sf::CircleShape>()) {}

sf::Sprite& SpriteComponent::getSprite() const { return *sprite; }

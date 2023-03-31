#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace std;

void SpriteComponent::setTexure(std::shared_ptr<sf::Texture> tex)
{
  _texture = tex;
  _sprite->setTexture(*_texture);
}


SpriteComponent::SpriteComponent(Entity* p)
    : Component(p), _sprite(make_shared<sf::Sprite>()) {}

void SpriteComponent::Update(double dt) {
  _sprite->setPosition(parent->getPosition());
  _sprite->setRotation(sf::degrees(parent->getRotation()));
}

void SpriteComponent::Render() { Renderer::Queue(_sprite.get()); }

void ShapeComponent::Update(double dt) {
  _shape->setPosition(parent->getPosition());
  _shape->setRotation(sf::degrees(parent->getRotation()));
}

void ShapeComponent::Render() { Renderer::Queue(_shape.get()); }

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p)
    : Component(p), _shape(make_shared<sf::CircleShape>()) {}

sf::Sprite& SpriteComponent::getSprite() const { return *_sprite; }

#pragma once

#include "ecm.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>

class SpriteComponent : public Component {
protected:
    std::shared_ptr<sf::Sprite> sprite;
    std::shared_ptr<sf::Texture> texture;
    sf::Vector2f size;
public:
    SpriteComponent() = delete;

    explicit SpriteComponent(Entity* p, sf::Vector2f pos);
    void Update(double dt) override;
    void Render() override;

    sf::Sprite& getSprite() const;


    void setTexure(std::shared_ptr<sf::Texture> tex);
};

class ShapeComponent : public Component {
protected:
    std::shared_ptr<sf::Shape> shape;
    // sf::Shape shape;

public:
    ShapeComponent() = delete;

    explicit ShapeComponent(Entity* p);

    void Update(double dt) override;
    void Render() override;
    sf::Shape& getShape() const;
    template <typename T, typename... Targs> void setShape(Targs... params) {
        shape.reset(new T(params...));
    }
};

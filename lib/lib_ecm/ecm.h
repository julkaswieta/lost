#pragma once
#include "maths.h"
#include <algorithm>
#include <memory>
#include <set>
#include <typeindex>
#include <vector>

class Entity;
class Scene;

class Component {
    friend Entity;

protected:
    Entity* const parent;
    bool forDeletion; // should be removed
    explicit Component(Entity* const p);

public:
    Component() = delete; // delete default constructor

    bool isForDeletion() const;

    virtual void Update(double dt) = 0;

    virtual void Render() = 0;

    virtual ~Component();
};

struct EntityManager {
    std::vector<std::shared_ptr<Entity>> list;
    void Update(double dt);
    void Render();
    std::vector<std::shared_ptr<Entity>>
        find(const std::string& tag) const;
    std::vector<std::shared_ptr<Entity>>
        find(const std::vector<std::string>& tags) const;
};

class Entity {
    friend struct EntityManager;

protected:
    std::vector<std::shared_ptr<Component>> components;
    sf::Vector2f position;
    float rotation;
    bool alive;       // should be updated
    bool visible;     // should be rendered
    bool forDeletion; // should be deleted
    std::set<std::string> tags;

public:
    Entity(Scene* const s);
    virtual ~Entity();

    Scene* const scene;

    virtual void Update(double dt);
    virtual void Render();
    
    void addTag(const std::string& t);
    const std::set<std::string>& getTags() const;
    
    const sf::Vector2f& getPosition() const;
    void setPosition(const sf::Vector2f& _position);

    bool isForDeletion() const;
    void setForDelete();

    float getRotation() const;
    void setRotation(float _rotation);

    bool isAlive() const;
    void setAlive(bool _alive);

    bool isVisible() const;
    void setVisible(bool _visible);

    template <typename T, typename... Targs>
    std::shared_ptr<T> addComponent(Targs... params) {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
        components.push_back(sp);
        return sp;
    }

    template <typename T>
    const std::vector<std::shared_ptr<T>> getComponents() const {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::vector<std::shared_ptr<T>> ret;
        for (const auto c : components) {
            if (typeid(*c) == typeid(T)) {
                ret.push_back(std::dynamic_pointer_cast<T>(c));
            }
        }
        return std::move(ret);
    }

    // Will return a T component, or anything derived from a T component.
    template <typename T>
    const std::vector<std::shared_ptr<T>> GetCompatibleComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::vector<std::shared_ptr<T>> ret;
        for (auto c : components) {
            auto dd = dynamic_cast<T*>(&(*c));
            if (dd) {
                ret.push_back(std::dynamic_pointer_cast<T>(c));
            }
        }
        return ret;
    }
};

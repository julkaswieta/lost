#include "ecm.h"

using namespace std;

// Entity

Entity::Entity(Scene* const s)
    : position({0, 0}), rotation(0), alive(true), visible(true),
      scene(s), forDeletion(false) {}


void Entity::Update(double dt) {
    if (!alive) {
        return;
    }
    for (size_t i = 0; i < components.size(); i++) {
        if (components[i]->isForDeletion()) {
            components.erase(components.begin() + i);
            --i;
        }
        components[i]->Update(dt);
    }
}

void Entity::Render() {
    if (!visible)
        return;
    for (auto& c : components)
        c->Render();
}

void Entity::addTag(const std::string& t) { tags.insert(t); }

const std::set<std::string>& Entity::getTags() const { return tags; }

const sf::Vector2f& Entity::getPosition() const { return position; }

void Entity::setPosition(const sf::Vector2f& _position) {
  Entity::position = _position;
}

float Entity::getRotation() const { return rotation; }

void Entity::setRotation(float _rotation) { Entity::rotation = _rotation; }

bool Entity::isAlive() const { return alive; }

void Entity::setAlive(bool _alive) { Entity::alive = _alive; }

bool Entity::isForDeletion() const { return forDeletion; }

void Entity::setForDelete() {
  forDeletion = true;
  alive = false;
  visible = false;
}

bool Entity::isVisible() const { return visible; }

void Entity::setVisible(bool _visible) { Entity::visible = _visible; }

Entity::~Entity() {
    // Components can inter-depend on each other, so deleting them may take
    // multiple passes. We Keep deleting components until we can't delete any
    // more
    int deli = 0;
    while (deli != components.size()) {
        deli = components.size();
        components.erase(
            remove_if(components.begin(), components.end(),
                [](auto& sp) { return (sp.use_count() <= 1); }),
            components.end());
    }

    if (components.size() > 0) {
        throw std::runtime_error(
            "Can't delete entity, someone is grabbing a component!");
    }

    components.clear();
}


// Component

Component::Component(Entity* const p) : parent(p), forDeletion(false) {}

bool Component::isForDeletion() const { return forDeletion; }

Component::~Component() {}


// EntityManager

// Update all entities in the manager
void EntityManager::Update(double dt) {
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i]->isForDeletion()) {
            list.erase(list.begin() + i);
            --i;
            continue;
        }
        if (list[i]->alive) {
            list[i]->Update(dt);
        }
    }
}

// Render all entities in the manager
void EntityManager::Render() {
    for (auto& e : list) {
        if (e->visible) {
            e->Render();
        }
    }
}

// find all entities with a specific tag in the maanager
vector<shared_ptr<Entity>> EntityManager::find(const string& tag) const {
    vector<shared_ptr<Entity>> ret;
    for (auto& e : list) {
        const auto tgs = e->tags;
        if (tgs.find(tag) != tgs.end()) {
            ret.push_back(e);
        }
    }
    return ret;
}

// find all entities with a group of specific tags 
vector<shared_ptr<Entity>>
EntityManager::find(const vector<string>& tags) const {
    vector<shared_ptr<Entity>> ret;
    for (auto& e : list) {
        const auto tgs = e->tags;
        if (any_of(tags.begin(), tags.end(),
            [&tgs](auto t) { return tgs.find(t) != tgs.end(); })) {
            ret.push_back(e);
        }
    }
    return ret;
}

#include "scene.h"

using namespace std;
using namespace sf;

// create and add entity to the scene
shared_ptr<Entity> Scene::makeEntity() {
	auto e = make_shared<Entity>(this);
	ents.list.push_back(e);
	return std::move(e);
}

void Scene::Update(const double& dt) { ents.Update(dt); }

void Scene::Render() { ents.Render(); }

bool Scene::isLoaded() const {
    {
        std::lock_guard<std::mutex> lck(loaded_mutex);
        // Are we already loading asynchronously?
        if (loaded_future.valid() // yes
            &&                     // Has it finished?
            loaded_future.wait_for(chrono::seconds(0)) ==
            future_status::ready) {
            // Yes
            loaded_future.get();
            loaded = true;
        }
        return loaded;
    }
}
void Scene::setLoaded(bool b) {
    {
        std::lock_guard<std::mutex> lck(loaded_mutex);
        loaded = b;
    }
}

void Scene::Unload() {
    ents.list.clear();
    setLoaded(false);
}

void Scene::LoadAsync() { loaded_future = std::async(&Scene::Load, this); }

Scene::~Scene() { Unload(); }
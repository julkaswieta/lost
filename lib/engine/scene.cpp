#include "scene.h"

using namespace sf;
using namespace std;

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
        std::lock_guard<std::mutex> lck(loadedMutex);
        // Are we already loading asynchronously?
        if (loadedFuture.valid() // yes
            &&                     // Has it finished?
            loadedFuture.wait_for(chrono::seconds(0)) ==
            future_status::ready) {
            // Yes
            loadedFuture.get();
            loaded = true;
        }
        return loaded;
    }
}
void Scene::setLoaded(bool b) {
    {
        std::lock_guard<std::mutex> lck(loadedMutex);
        loaded = b;
    }
}

void Scene::Unload() {
    ents.list.clear();
    setLoaded(false);
}

void Scene::LoadAsync() { loadedFuture = std::async(&Scene::Load, this); }

Scene::~Scene() { Unload(); }

#pragma once
#include "ecm.h"
#include <mutex>
#include <future>

class Scene {
public:
	Scene() = default;
	virtual ~Scene();
	virtual void Load() = 0;
	virtual void LoadAsync();
	virtual void Unload();
	virtual void Update(const double& dt);
	virtual void Render();
	bool isLoaded() const;
	std::shared_ptr<Entity> makeEntity();

	EntityManager ents;

protected:
	void setLoaded(bool);
private:
	mutable bool loaded;
	mutable std::future<void> loadedFuture;
	mutable std::mutex loadedMutex;
};

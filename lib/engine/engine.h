#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <maths.h>
#include <string>
#include "scene.h"

class Engine {
public:
    Engine() = delete;
    static void Start(const std::string& gameName, Scene* scn);
    static void ChangeScene(Scene*);
    static sf::RenderWindow& getWindow();
    static sf::Vector2u getWindowSize();
    static void setVsync(bool b);
    static std::string gameName;
    static bool paused;

private:
    static Scene* activeScene;
    static void Update();
    static void Render(sf::RenderWindow& window);
    static void calculateFps(double dt);
    static void handleStandardEvents(sf::RenderWindow& rw);
};

namespace timing {
    // Return time since Epoc
    long long now();
    // Return time since last() was last called.
    long long last();
} // namespace timing

namespace loading {
    void loadingUpdate(float dt, const Scene* const scn);
    void loadingRender();
}

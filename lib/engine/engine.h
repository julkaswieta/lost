#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <maths.h>
#include <string>
#include "scene.h"

class Engine {
public:
    Engine() = delete;
    static void Start(unsigned int width, unsigned int height,
        const std::string& gameName, Scene* scn);
    static void ChangeScene(Scene*);
    static sf::RenderWindow& getWindow();
    static sf::Vector2u getWindowSize();
    static void setVsync(bool b);

private:
    static Scene* activeScene;
    static std::string gameName;
    static void Update();
    static void Render(sf::RenderWindow& window);
    static void calculateFps(double dt);
};

namespace timing {
    // Return time since Epoc
    long long now();
    // Return time since last() was last called.
    long long last();
} // namespace timing
#include "engine.h"

#include <future>
#include <iostream>
#include <stdexcept>

#include "../src/save_system.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"

using namespace sf;
using namespace std;

Scene* Engine::activeScene = nullptr;
string Engine::gameName;
static RenderWindow* window;

static bool currentlyLoading = false;
static float loadingSpinner = 0.f;
static float loadingTime;

bool Engine::paused = false;

float frametimes[256] = {};
uint8_t frameTimesCounter = 0;

// Initialises the game's engine and all subsystems
void Engine::Start(const std::string& gameName, Scene* scn) {
    SaveSystem::initialiseSaveSystem();
    SaveSystem::loadSettings();
    SaveSystem::loadGame();
    Vector2u targetResolution = SaveSystem::getResolution();
    int targetWindowMode = SaveSystem::getWindowMode();
    RenderWindow rw(VideoMode({ targetResolution.x, targetResolution.y}), gameName, (targetWindowMode == 0) ? Style::Fullscreen : Style::Default);
    Engine::gameName = gameName;
    window = &rw;

    View view(FloatRect(Vector2f(0.f, 0.f), Vector2f(1920.f, 1080.f)));
    view.setCenter(Vector2f(targetResolution.x / 2.f, targetResolution.y / 2.f));
    rw.setView(view);

    Renderer::Initialise(rw);
    Physics::Initialise();
    ChangeScene(scn);

    while (rw.isOpen()) {
        handleStandardEvents(rw);
        rw.clear();
        Update();
        Render(rw);
        rw.display();
    }

    if (activeScene != nullptr) {
        activeScene->Unload();
        activeScene = nullptr;
    }

    rw.close();
    Physics::Shutdown();
    // Renderer::shutdown();
}

// changes the active scene (unloads previous and loads new)
void Engine::ChangeScene(Scene* s) {
    auto old = activeScene;
    activeScene = s;

    if (old != nullptr) {
        old->Unload(); // todo: Unload Async
    }

    if (!s->isLoaded()) {
        loadingTime = 0;
        activeScene->LoadAsync();
        //activeScene->Load();
        currentlyLoading = true;
    }
}

// Update for the whole engine - calls all other updates
void Engine::Update() {
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();
    calculateFps(dt);

    // display loading screen if loading
    if (currentlyLoading) {
        loading::loadingUpdate(dt, activeScene);
    }
    else if (activeScene != nullptr) {  // else update active scene
        Physics::Update(dt);
        activeScene->Update(dt);
    }
}

// Render for the whole engine, calls all other enders
void Engine::Render(RenderWindow& window) {
    if (currentlyLoading) {   // render loading screen if loading 
        loading::loadingRender();
    }
    else if (activeScene != nullptr) { // else render active scene
        activeScene->Render();
    }

    Renderer::Render();
}



// Calculates fps and displays in title bar
void Engine::calculateFps(double dt) {
    frametimes[++frameTimesCounter] = dt;
    static string fpsMessage = gameName + " FPS:";
    // every 60 frames, calculate the average fps value
    if (frameTimesCounter % 60 == 0) {
        double avgDelta = 0;
        for (const auto t : frametimes) {
            avgDelta += t;  // sum delta times
        }
        avgDelta = 1.0 / (avgDelta / 255.0);

        window->setTitle(fpsMessage + toStrDecPt(2, avgDelta));
    }
}

// Handle close window reqests etc.
void Engine::handleStandardEvents(RenderWindow& rw) {
    Event event;
    while (rw.pollEvent(event)) {
        if (event.type == Event::Closed) {
            rw.close();
        }
    }
}

void Engine::setVsync(bool b) { window->setVerticalSyncEnabled(b); }

sf::Vector2u Engine::getWindowSize() { return window->getSize(); }

sf::RenderWindow& Engine::getWindow() { return *window; }

namespace timing {
    // Return time since Epoc
    long long now() {
        return std::chrono::high_resolution_clock::now()
            .time_since_epoch()
            .count();
    }
    // Return time since last() was last called.
    long long last() {
        auto n = now();
        static auto then = now();
        auto dt = n - then;
        then = n;
        return dt;
    }
} // namespace timing

namespace loading {
    // update for loading screen
    void loadingUpdate(float dt, const Scene* const scn) {
        if (scn->isLoaded()) {
            currentlyLoading = false;
        }
        else {
            loadingSpinner += 220.0f * dt;
            loadingTime += dt;
        }
    }

    // render for loading screen
    void loadingRender() {
        static CircleShape octagon(80, 8);
        octagon.setOrigin(Vector2f(80, 80));
        octagon.setRotation(degrees(loadingSpinner));
        octagon.setPosition(Vcast<float>(Engine::getWindowSize()) * .5f);
        octagon.setFillColor(Color(255, 255, 255, min(255.f, 40.f * loadingTime)));
        static Text t("Loading", *Resources::get<sf::Font>("RobotoMono-Regular.ttf"));
        t.setFillColor(Color(255, 255, 255, min(255.f, 40.f * loadingTime)));
        t.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.3f));
        Renderer::Queue(&t);
        Renderer::Queue(&octagon);
    }
}

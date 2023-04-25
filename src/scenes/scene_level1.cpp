#include "scene_level1.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "../controls.h"

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level1Scene::Load() {
    cout << " Scene 1 Load" << endl;
    ls::LoadLevelFile("res/levels/TestLevelV2.csv", 60.0f);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 60.f);
    ls::setOffset(Vector2f(0, ho));

    // Create player
    {
        player = makeEntity();
        auto startTile = ls::findTiles(ls::START)[0];
        auto startPos = ls::getTilePosition(startTile);
        player->setPosition(startPos);
        auto s = player->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(40.f, 60.f));
        s->getShape().setFillColor(Color::Magenta);
        s->getShape().setOrigin(Vector2f(20.f, 30.f));
        player->addTag("player");
        player->addComponent<PlayerPhysicsComponent>(Vector2f(40.f, 60.f));
    }

    // Add physics colliders to level tiles
    {
        auto walls = ls::findTiles(ls::WALL);
        for (auto w : walls) {
            auto pos = ls::getTilePosition(w);
            pos += Vector2f(30.f, 30.f); //offset to center
            auto e = makeEntity();
            e->setPosition(pos);
            e->addComponent<PhysicsComponent>(false, Vector2f(60.f, 60.f));
        }
    }

    // Add hurt components to hazard tiles
    {
        auto spikes = ls::findTiles(ls::SPIKE_UP);
        for (auto s : spikes) {
            auto h = makeEntity();
            auto pos = ls::getTilePosition(s) + Vector2f(30.f, 30.f); //offset to center
            h->setPosition(pos);
            h->addComponent<HurtComponent>();
        }
    }

    // Add debug text
    {
        auto txt = makeEntity();
        auto t = txt->addComponent<TextComponent>("debug");
        t->SetColor(Color::Black);
    }

    //Simulate long loading times to check loading screen works
    //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    cout << " Scene 1 Load Done" << endl;

    setLoaded(true);
}

void Level1Scene::Unload() {
    cout << "Scene 1 Unload" << endl;
    player.reset();
    ls::Unload();
    Scene::Unload();
}

void Level1Scene::Update(const double& dt) {
    if (Keyboard::isKeyPressed(Controls::Exit)) {
        //Engine::ChangeScene(())
    }
    if (ls::getTileAt(player->getPosition()) == ls::END) {
        Engine::ChangeScene((Scene*)&menu);
    }
    else if (!player->isAlive())
    {
        Engine::ChangeScene((Scene*)&level1);
    }
    Scene::Update(dt);
}

void Level1Scene::Render() {
    ls::Render(Engine::getWindow());
    Scene::Render();
}

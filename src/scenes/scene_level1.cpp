#include "../game.h"
#include "../controls.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "scene_level1.h"

#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player, blob;

void Level1Scene::Load() {
    cout << " Scene 1 Load" << endl;
    ls::LoadLevelFile("res/levels/test_level_v3.txt", 60.0f);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 60.f);
    ls::setOffset(Vector2f(0, ho));

    // Create player
    {
        player = makeEntity();
        player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
        auto s = player->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(40.f, 60.f));
        s->getShape().setFillColor(Color::Magenta);
        s->getShape().setOrigin(Vector2f(20.f, 30.f));
        player->addTag("player");
        player->addComponent<PlayerPhysicsComponent>(Vector2f(40.f, 60.f));
    }

    // Create blob
    {
        blob = makeEntity();
        blob->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]));
        auto s = blob->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(60.f, 40.f));
        s->getShape().setFillColor(Color::Blue);
        s->getShape().setOrigin(Vector2f(30.f, 20.f));
        blob->addTag("blob");
        //blob->addComponent<PhysicsComponent>(true, Vector2f(60.f, 40.f));
        blob->addComponent<HurtComponent>();
        auto ai = blob->addComponent<EnemyAIComponent>();
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
        vector<sf::Vector2ul> hazards;

        for (auto su : ls::findTiles(ls::SPIKE_UP))
        hazards.push_back(su);
        
        for (auto sd : ls::findTiles(ls::SPIKE_DOWN))
        hazards.push_back(sd);
        
        for (auto sr : ls::findTiles(ls::SPIKE_RIGHT))
        hazards.push_back(sr);
        
        for (auto sl : ls::findTiles(ls::SPIKE_LEFT))
        hazards.push_back(sl);
        
        for (auto sl : ls::findTiles(ls::SPIKE_BALL))
        hazards.push_back(sl);
        
        for (auto sl : ls::findTiles(ls::SAWBLADE))
        hazards.push_back(sl);

        for (auto s : hazards) {
            auto h = makeEntity();
            auto pos = ls::getTilePosition(s) + Vector2f(30.f, 30.f); // Offset to center
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

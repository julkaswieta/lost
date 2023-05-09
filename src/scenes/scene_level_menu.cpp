/**
* scene_level_1menucpp: implementation for LevelMenuScene class
*
* Author: Dillon Aitken
* 
* Last modified: 09/05/2023
*/
#include "scene_level_menu.h"

#include <thread>
#include <iostream>
#include <system_resources.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <LevelSystem.h>

#include "../game.h"
#include "../controls.h"
#include "../save_system.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_game_sounds.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_spike_ball.h"

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void LevelMenuScene::Load() {
	std::cout << " Scene Menu Load" << endl;
    ls::LoadLevelFile("res/levels/level_menu.txt", 60.0f);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 60.f);
    ls::setOffset(Vector2f(0, ho));

    // Load sounds
    {
        auto gameSounds = makeEntity();
        gameSounds->addTag("gameSounds");
        gameSounds->addComponent<GameSoundsComponent>();
    }

    auto defaultSize = Vector2f(60.f, 60.f);

    // Add sprites to settings and level start tiles
    {
        for (auto ts : ls::findTiles(ls::SETTINGS)) {
            auto s = makeEntity();
            auto pos = ls::getTilePosition(ts) + Vector2f(30.f, 30.f);
            s->setPosition(pos);
            auto sc = s->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("Settings.png"));
        }

        for (auto ts : ls::findTiles(ls::LEVEL1)) {
            auto s = makeEntity();
            auto pos = ls::getTilePosition(ts) + Vector2f(30.f, 30.f);
            s->setPosition(pos);
            auto sc = s->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("Level1.png"));
        }

        for (auto ts : ls::findTiles(ls::LEVEL2)) {
            auto s = makeEntity();
            auto pos = ls::getTilePosition(ts) + Vector2f(30.f, 30.f);
            s->setPosition(pos);
            auto sc = s->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("Level2.png"));
        }

        for (auto ts : ls::findTiles(ls::LEVEL3)) {
            auto s = makeEntity();
            auto pos = ls::getTilePosition(ts) + Vector2f(30.f, 30.f);
            s->setPosition(pos);
            auto sc = s->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("Level3.png"));
        }
    }

    // Create player
    {
        b2Filter playerFilter;
        playerFilter.categoryBits = 0x0002;
        playerFilter.maskBits = 0x0008;

        player = makeEntity();
        player->addTag("player");
        player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
        player->addComponent<PlayerPhysicsComponent>(Vector2f(30.f, 60.f));
        player->getComponents<PlayerPhysicsComponent>()[0]->getFixture()->SetFilterData(playerFilter);
        auto sprite = player->addComponent<SpriteComponent>(Vector2f(40.f, 60.f));
        sprite->setTexure(Resources::get<Texture>("PlayerWalkRight2.png"));
    }

    // Add hurt components and sprites to hazard tiles
    {
        for (auto su : ls::findTiles(ls::SPIKE_UP)){
            auto h = makeEntity();
            auto pos = ls::getTilePosition(su) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(50.f);
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeUp.png"));
        }

        for (auto sd : ls::findTiles(ls::SPIKE_DOWN)) {
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sd) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(50.f);
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeDown.png"));
        }

        for (auto sr : ls::findTiles(ls::SPIKE_RIGHT)) {
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sr) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(50.f);
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeRight.png"));
        }

        for (auto sl : ls::findTiles(ls::SPIKE_LEFT)) {
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sl) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(50.f);
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeLeft.png"));
        }

        b2Filter spikeBallFilter;
        spikeBallFilter.categoryBits = 0x0006;
        spikeBallFilter.maskBits = 0x0008;

        for (auto sball : ls::findTiles(ls::SPIKE_BALL)) {
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sball) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(55.f);
            h->addComponent<SpikeBallComponent>(Vector2f(60.f, 60.f));
            h->getComponents<SpikeBallComponent>()[0]->getFixture()->SetFilterData(spikeBallFilter);
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeBall.png"));
        }

        for (auto sblade : ls::findTiles(ls::SAWBLADE)) {
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sblade) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(50.f);
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeBase.png"));
        }
    }

    // Add physics colliders to level tiles
    {
        b2Filter wallFilter;
        wallFilter.categoryBits = 0x0008;
        wallFilter.maskBits = 0x0002 | 0x0004 | 0x0006;

        auto walls = ls::findTiles(ls::WALL);
        for (auto w : walls) {
            auto pos = ls::getTilePosition(w);
            pos += Vector2f(30.f, 30.f); //offset to center
            auto e = makeEntity();
            e->addTag("wall");
            e->setPosition(pos);
            e->addComponent<PhysicsComponent>(false, Vector2f(60.f, 60.f));
            e->getComponents<PhysicsComponent>()[0]->getFixture()->SetFilterData(wallFilter);
        }
    }

    // Temporary death tracker
    {
        auto deathTracker = makeEntity();
        deathTracker->addTag("deathTracker");
        deathTracker->setPosition(Vector2f(210.f, 90.f));
        auto text = deathTracker->addComponent<TextComponent>("Total Deaths: " +
            to_string(SaveSystem::getDeathCount()));
        text->SetColor(Color::Black);
        text->getText().setOrigin(Vector2f(0.f, text->getText().getLocalBounds().height * 0.5f));
        text->getText().setPosition(Vector2f(120.f, 90.f));
    }

    // Add exit instructions
    {
        auto exitText = makeEntity();
        exitText->addTag("exitText");
        exitText->setPosition(Vector2f(Engine::getWindowSize().x - 180.f, 90.f));
        auto text = exitText->addComponent<TextComponent>(
            "Press Esc to exit the game."
        );
        text->SetColor(Color::Black);
        text->getText().setOrigin(Vector2f(
            text->getText().getLocalBounds().width,
            text->getText().getLocalBounds().height * 0.5f
        ));
        text->getText().setPosition(Vector2f(Engine::getWindowSize().x - 120.f, 90.f));
    }

    //Simulate long loading times to check loading screen works
    //this_thread::sleep_for(chrono::milliseconds(3000));
    std::cout << " Scene Menu Load Done" << endl;

    setLoaded(true);
    Engine::paused = false;
}

void LevelMenuScene::Unload() {
    std::cout << "Scene Menu Unload" << endl;
    player.reset();
    ls::Unload();
    Scene::Unload();
}

void LevelMenuScene::Update(const double& dt) {
    if (!Engine::paused) {
        ents.find("deathTracker")[0]->getComponents<TextComponent>()[0]->
			SetText("Total Deaths: " + to_string(SaveSystem::getDeathCount()));

        if (ls::getTileAt(player->getPosition()) == ls::SETTINGS) {
            Engine::ChangeScene((Scene*)&settings);
        }
        else if (ls::getTileAt(player->getPosition()) == ls::LEVEL1) {
            Engine::ChangeScene((Scene*)&level1);
        }
        else if (ls::getTileAt(player->getPosition()) == ls::LEVEL2) {
            Engine::ChangeScene((Scene*)&level2);
        }
        else if (ls::getTileAt(player->getPosition()) == ls::LEVEL3) {
            Engine::ChangeScene((Scene*)&level1);
        }
        else if (!player->isAlive()) {
            SaveSystem::setDeathCounter(SaveSystem::getDeathCount() + 1);
            SaveSystem::saveGame();
            Engine::ChangeScene((Scene*)&levelMenu);
        }
        if (Keyboard::isKeyPressed(Controls::Exit) || 
            sf::Joystick::isButtonPressed(0,7)) {
            Engine::getWindow().close();
        }
    }
    Scene::Update(dt);
}

void LevelMenuScene::Render() {
    ls::Render(Engine::getWindow());
    Scene::Render();
}

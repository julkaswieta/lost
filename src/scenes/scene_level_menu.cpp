/**
* scene_level_menu.cpp: implementation for LevelMenuScene class
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

// Define the player entity
static shared_ptr<Entity> player;

void LevelMenuScene::Load() {
    // Load the level file and set the offset for level rendering
	std::cout << " Scene Menu Load" << endl;
    ls::LoadLevelFile("res/levels/level_menu.txt", 60.0f);
    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 60.f);
    ls::setOffset(Vector2f(0, ho));

    // Load sounds
    {
        // Add a sound component to the scene for playing sounds
        auto gameSounds = makeEntity();
        gameSounds->addTag("gameSounds");
        gameSounds->addComponent<GameSoundsComponent>();
    }

    // Define the default size for sprites
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

    // Create the player entity and add the necessary components
    {
        // Set collision filtering for the player
        b2Filter playerFilter;
        playerFilter.categoryBits = 0x0002;
        playerFilter.maskBits = 0x0008;

        // Create the player entity and set its position
        player = makeEntity();
        player->addTag("player");
        player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));

        // Add the player physics component and set the fixture filter data
        player->addComponent<PlayerPhysicsComponent>(Vector2f(30.f, 60.f));
        player->getComponents<PlayerPhysicsComponent>()[0]->getFixture()->SetFilterData(playerFilter);

        // Add the sprite component and set the texture for the player entity
        player->addComponent<SpriteComponent>(Vector2f(40.f, 60.f));
    }

    // Add hurt components and sprites to hazard tiles
    {
        // Find all tiles with hazard spikes pointing up and create a new entity for each
        for (auto su : ls::findTiles(ls::SPIKE_UP)){
            // Set position, tag as "hazard", and add a HurtComponent with radius of 50
            auto h = makeEntity();
            auto pos = ls::getTilePosition(su) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(50.f);
            // Add a SpriteComponent with texture "SpikeUp.png"
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeUp.png"));
        }

        // Find all tiles with hazard spikes pointing down and create a new entity for each
        for (auto sd : ls::findTiles(ls::SPIKE_DOWN)) {
            // Set position, tag as "hazard", and add a HurtComponent with radius of 50
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sd) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(50.f);
            // Add a SpriteComponent with texture "SpikeDown.png"
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeDown.png"));
        }

        // Find all tiles with hazard spikes pointing right and create a new entity for each
        for (auto sr : ls::findTiles(ls::SPIKE_RIGHT)) {
            // Set position, tag as "hazard", and add a HurtComponent with radius of 50
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sr) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos); 
            h->addComponent<HurtComponent>(50.f);
            // Add a SpriteComponent with texture "SpikeRight.png"
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeRight.png"));
        }

        // Find all tiles with hazard spikes pointing left and create a new entity for each
        for (auto sl : ls::findTiles(ls::SPIKE_LEFT)) {
            // Set position, tag as "hazard", and add a HurtComponent with radius of 50
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sl) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(50.f);
            // Add a SpriteComponent with texture "SpikeLeft.png"
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeLeft.png"));
        }

        // Set the filter category and mask bits for the spike ball entity
        b2Filter spikeBallFilter;
        spikeBallFilter.categoryBits = 0x0006;
        spikeBallFilter.maskBits = 0x0008;

        // Find all tiles with hazard spike balls and create a new entity for each
        for (auto sball : ls::findTiles(ls::SPIKE_BALL)) {
            // Set position, tag as "hazard", and add a HurtComponent with radius of 50
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sball) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(55.f);
            h->addComponent<SpikeBallComponent>(Vector2f(60.f, 60.f));
            h->getComponents<SpikeBallComponent>()[0]->getFixture()->SetFilterData(spikeBallFilter);
            // Add a SpriteComponent with texture "SpikeBall.png"
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeBall.png"));
        }

        // Find all tiles with hazard sawblades and create a new entity for each
        for (auto sblade : ls::findTiles(ls::SAWBLADE)) {
            // Set position, tag as "hazard", and add a HurtComponent with radius of 50
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sblade) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(50.f);
            // Add a SpriteComponent with texture "SpikeBase.png"
            auto sc = h->addComponent<SpriteComponent>(defaultSize);
            sc->setTexure(Resources::get<sf::Texture>("SpikeBase.png"));
        }
    }

    // Add physics colliders to level tiles
    {
        // Set the filter category and mask bits for the level tiles
        b2Filter wallFilter;
        wallFilter.categoryBits = 0x0008;
        wallFilter.maskBits = 0x0002 | 0x0004 | 0x0006;

        // Find all wall tiles and create a new entity for each
        auto walls = ls::findTiles(ls::WALL);
        for (auto w : walls) {
            // Set position and tag as wall
            auto pos = ls::getTilePosition(w);
            pos += Vector2f(30.f, 30.f); //offset to center
            auto e = makeEntity();
            e->addTag("wall");
            e->setPosition(pos);
            // Add a PhysicsComponent with a size of 60x60 and set the filter data
            e->addComponent<PhysicsComponent>(false, Vector2f(60.f, 60.f));
            e->getComponents<PhysicsComponent>()[0]->getFixture()->SetFilterData(wallFilter);
        }
    }

    // Total death tracker
    {
        // Create a new entity and add a TextComponent to display the total deaths
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
        // Create a new entity and add a TextComponent to display the exit instructions
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

    // Set loaded flag to true
    setLoaded(true);
    // Make sure the engine is not paused
    Engine::paused = false;
}

void LevelMenuScene::Unload() {
    // Unload all entities
    std::cout << "Scene Menu Unload" << endl;
    player.reset();
    ls::Unload();
    Scene::Unload();
}

void LevelMenuScene::Update(const double& dt) {
    if (!Engine::paused) {
        // update the death count text on the death tracker entity
        ents.find("deathTracker")[0]->getComponents<TextComponent>()[0]->
            SetText("Total Deaths: " + to_string(SaveSystem::getDeathCount()));

        // check the current tile the player is on and change the scene accordingly
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
            // increment death count and save the game, then return to the level menu scene
            SaveSystem::setDeathCounter(SaveSystem::getDeathCount() + 1);
            SaveSystem::saveGame();
            Engine::ChangeScene((Scene*)&levelMenu);
        }
        // check for keyboard or joystick input to exit the game
        if (Keyboard::isKeyPressed(Controls::Exit) ||
            sf::Joystick::isButtonPressed(0, 7)) {
            Engine::getWindow().close();
        }
    }
    // update the scene as usual
    Scene::Update(dt);
}

void LevelMenuScene::Render() {
    // render the level tiles
    ls::Render(Engine::getWindow());
    // render all other entities
    Scene::Render();
}

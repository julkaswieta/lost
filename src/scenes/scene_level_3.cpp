/**
* scene_level_3.cpp: implementation for Level3Scene class
*
* Author: Dillon Aitken
* Pause Menu: Julia Swietochowska
* Last modified: 09/05/2023
*/
#include "scene_level_3.h"

#include <thread>
#include <iostream>
#include <system_resources.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <LevelSystem.h>

#include "../game.h"
#include "../controls.h"
#include "../save_system.h"
#include "../components/cmp_text.h"
#include "../components/cmp_blob.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_spike_ball.h"
#include "../components/cmp_collectible.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_game_sounds.h"
#include "../components/cmp_player_physics.h"

using namespace std;
using namespace sf;

static shared_ptr<Entity> player, blob;
vector<shared_ptr<Entity>> Level3Scene::menuOptions;
float Level3Scene::timer;

void Level3Scene::Load() {
    std::cout << "Level 3 Load" << endl;
    ls::LoadLevelFile("res/levels/level_3.txt", 60.0f);

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

    // Add components and sprites to star tiles depending 
    // on whether they have been collected or not
    {
        // Clear the previously collected stars
        collected.clear();

        // For each collected string in SaveSystem::Collected,
        // add it to collected vector if it starts with "Level1"
        for (string s : SaveSystem::getCollected())
            if (s.rfind("Level1_", 0) == 0)
                collected.push_back(s);

        // Find all tiles tagged as "STAR" in the level
        auto stars = ls::findTiles(ls::STAR);
        for (int i = 0; i < 3; i++) {
            // Check if star i+1 has been collected
            std::cout << "Checking for Level1_" << i + 1 << endl;
            if (find(collected.begin(), collected.end(), "Level1_" + to_string(i + 1)) != collected.end()) {
                // If star i+1 is in collected vector, it has been collected
                std::cout << "Star " << i + 1 << " collected" << endl;
            }
            else {
                // If star i+1 has not been collected, add a collectible entity to the level
                std::cout << "Star " << i + 1 << " not collected. Adding to level..." << endl;
                // Create a new entity and set position
                auto s = makeEntity();
                s->addTag("Level1_" + to_string(i + 1));
                s->setPosition(ls::getTilePosition(stars[i]) + Vector2f(30.f, 30.f));
                // Add a CollectibleComponent to the entity
                s->addComponent<CollectibleComponent>(55.f);
                // Add a SpriteComponent with the texture "Star.png"
                auto sc = s->addComponent<SpriteComponent>(defaultSize);
                sc->setTexure(Resources::get<sf::Texture>("Star.png"));
            }
        }
    }

    // Add components and sprites to goal tile
    {
        auto goal = ls::findTiles(ls::END)[0];
        auto g = makeEntity();
        g->addTag("goal");
        g->setPosition(ls::getTilePosition(goal) + Vector2f(30.f, 30.f));
        auto sprite = g->addComponent<SpriteComponent>(defaultSize);
        sprite->setTexure(Resources::get<sf::Texture>("Goal.png"));
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

    // Create blob
    {
        // Set the filter category and mask bits for the blob entity
        b2Filter blobFilter;
        blobFilter.categoryBits = 0x0004;
        blobFilter.maskBits = 0x0008;

        // Create a new entity and set position
        blob = makeEntity();
        blob->addTag("blob");
        blob->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]) + Vector2f(30.f, 40.f));
        // Add a SpriteComponent with size 50x40
        blob->addComponent<SpriteComponent>(Vector2f(50.f, 40.f));
        // Add a HurtComponent with radius of 50
        blob->addComponent<HurtComponent>(50.f);
        // Add BlobComponent with a size of 50x40 and set the filter data
        blob->addComponent<BlobComponent>(Vector2f(50.f, 40.f));
        blob->getComponents<BlobComponent>()[0]->getFixture()->SetFilterData(blobFilter);
    }

    // Add star counter in the top left of screen
    {
        auto starTracker = makeEntity();
        starTracker->addTag("starTracker");
        starTracker->setPosition(Vector2f(150.f, 90.f));
        auto sprite = starTracker->addComponent<SpriteComponent>(defaultSize);
        sprite->setTexure(Resources::get<sf::Texture>("Star.png"));
        auto text = starTracker->addComponent<TextComponent>("0/3");
        text->SetColor(Color::Black);
        text->getText().setOrigin(Vector2f(0.f, text->getText().getLocalBounds().height * 0.5f));
        text->getText().setPosition(Vector2f(190.f, 90.f));
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
        text->getText().setPosition(Vector2f(300.f, 90.f));
    }

    // Add timeTracker in top right of screen
    {
        timer = 0.f;
        bestTime = SaveSystem::getLevelBestTime(3);

        if (bestTime <= 0.f)
            bestTimeString = "--:--:--";
        else
            bestTimeString = timeToString(bestTime);

        auto timeTracker = makeEntity();
        timeTracker->addTag("timeTracker");
        timeTracker->setPosition(Vector2f(Engine::getWindowSize().x - 180.f, 90.f));
        auto text = timeTracker->addComponent<TextComponent>(
            "Best: 00:00:00 | Current: 00:00:00"
        );
        text->SetColor(Color::Black);
        text->getText().setOrigin(Vector2f(
            text->getText().getLocalBounds().width,
            text->getText().getLocalBounds().height * 0.5f
        ));
        text->getText().setPosition(Vector2f(Engine::getWindowSize().x - 120.f, 90.f));
    }

    loadPauseMenu();

    //Simulate long loading times to check loading screen works
    //this_thread::sleep_for(chrono::milliseconds(3000));
    std::cout << "Level 3 Load Done" << endl;

    setLoaded(true);
}

void Level3Scene::AddCollected(string tag) {
    collected.push_back(tag);
}

string Level3Scene::timeToString(float s) {
    string minutes = to_string((int)s / 60);
    if (minutes.length() == 1) minutes = "0" + minutes;
    string seconds = to_string((int)s % 60);
    if (seconds.length() == 1) seconds = "0" + seconds;
    string milliseconds = to_string((int)(s * 100) % 100);
    if (milliseconds.length() == 1) milliseconds = "0" + milliseconds;

    return minutes + ":" + seconds + ":" + milliseconds;
}

void Level3Scene::loadPauseMenu() {
    auto background = makeEntity();
    auto shapeCmp = background->addComponent<ShapeComponent>();
    shapeCmp->setShape<sf::RectangleShape>(Vector2f(500.f, 500.f));
    shapeCmp->getShape().setFillColor(Color::Black);
    shapeCmp->getShape().setOutlineThickness(10.f);
    shapeCmp->getShape().setOutlineThickness(10.f);
    shapeCmp->getShape().setOutlineColor(Color::White);
    shapeCmp->getShape().setOrigin(Vector2(shapeCmp->getShape().getLocalBounds().width * 0.5f, shapeCmp->getShape().getLocalBounds().height * 0.5f));
    background->setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, Engine::getWindowSize().y * 0.5f));
    background->addTag("background");
    background->setVisible(false);

    string optionsText[4] = { "Game Paused", "Resume", "Restart Level", "Exit to Main Menu" };
    for (int i = 0; i < 4; ++i) {
        auto menuOption = makeEntity();
        auto textCmp = menuOption->addComponent<TextComponent>(optionsText[i]);
        textCmp->getText().setOrigin(Vector2f(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
        textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, 400 + ((i + 1) * 50)));
        menuOption->addTag("menu");
        menuOption->setVisible(false);
        menuOption->setAlive(false);
        if (i > 0)
            menuOptions.push_back(menuOption);
    }
    selectedOptionIndex = -1;
    Engine::paused = false;
}

void Level3Scene::Unload() {
    menuOptions.clear();
    std::cout << "Level 3 Unload" << endl;
    player.reset();
    blob.reset();
    ls::Unload();
    Scene::Unload();
}

void Level3Scene::Update(const double& dt) {
    if (!Engine::paused) {
        ents.find("starTracker")[0]->getComponents<TextComponent>()[0]->
            SetText(to_string(collected.size()) + "/3");

        ents.find("deathTracker")[0]->getComponents<TextComponent>()[0]->
			SetText("Deaths: " + to_string(SaveSystem::getDeathCount()));

        timer += dt;
        ents.find("timeTracker")[0]->getComponents<TextComponent>()[0]->
            SetText("Best: " + bestTimeString + " | Current: " + timeToString(timer));

        if (ls::getTileAt(player->getPosition()) == ls::END) {
            SaveSystem::addCollected(collected);

            if (bestTime <= 0.f || timer < bestTime) {
				bestTime = timer;
				bestTimeString = timeToString(bestTime);
                SaveSystem::addNewLevelTime(3, timer);
			}

            SaveSystem::setLastLevelCompleted(3);
            SaveSystem::saveGame();
            Engine::ChangeScene((Scene*)&endLevel);
        }
        else if (!player->isAlive()) {
            SaveSystem::setDeathCounter(SaveSystem::getDeathCount() + 1);
            SaveSystem::saveGame();
            Engine::ChangeScene((Scene*)&level3);
        }

        if (Keyboard::isKeyPressed(Controls::Exit) || 
            sf::Joystick::isButtonPressed(0, 7)) {
            Engine::paused = true;
            displayMenu();
        }
    }
    // pause menu update
    else {
        if (Keyboard::isKeyPressed(Controls::MenuDown) || 
            sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) <= -20) {
            moveDown();
        }
        if (Keyboard::isKeyPressed(Controls::MenuUp) || 
            sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) >= 20) {
            moveUp();
        }
        if (Keyboard::isKeyPressed(Controls::MenuSelect) || 
            sf::Joystick::isButtonPressed(0, 0)) {
            executeSelectedOption();
        }
    }
    Scene::Update(dt);
}

void Level3Scene::Render() {
    ls::Render(Engine::getWindow());
    Scene::Render();
}

void Level3Scene::displayMenu() {
    ents.find("background")[0]->setVisible(true);
    auto menu = ents.find("menu");
    for (auto menuoption : menu) {
        menuoption->setVisible(true);
        menuoption->setAlive(true);
    }
    ents.find("player")[0]->setAlive(false);
}

void Level3Scene::hideMenu() {
    Engine::paused = false;
    ents.find("background")[0]->setVisible(false);
    auto menu = ents.find("menu");
    for (auto menuoption : menu) {
        menuoption->setVisible(false);
        menuoption->setAlive(false);
    }
    ents.find("player")[0]->setAlive(true);
}

void Level3Scene::moveUp() {
    if (selectedOptionIndex - 1 >= 0) {
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::White);
        selectedOptionIndex--;
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        this_thread::sleep_for(chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
}

void Level3Scene::moveDown() {
    // handle initial state when nothing is selected
    if (selectedOptionIndex == -1) {
        selectedOptionIndex = 0;
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        this_thread::sleep_for(chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
    else if (selectedOptionIndex + 1 < menuOptions.size()) {
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::White);
        selectedOptionIndex++;
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        this_thread::sleep_for(chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
}
void Level3Scene::executeSelectedOption() {
    switch (selectedOptionIndex) {
    case 0:
        hideMenu();
        break;
    case 1:
        Engine::ChangeScene((Scene*)&level3);
        break;
    case 2:
        Engine::ChangeScene(&levelMenu);
    }
}

float Level3Scene::getTimer() {
    return timer;
}

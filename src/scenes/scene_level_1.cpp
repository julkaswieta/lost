/**
* scene_level_1.cpp: implementation for Level1Scene class
*
* Author: Dillon Aitken
* Pause Menu: Julia Swietochowska
* Last modified: 09/05/2023
*/
#include "scene_level_1.h"

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
vector<shared_ptr<Entity>> Level1Scene::menuOptions;
float Level1Scene::timer;

void Level1Scene::Load() {
    // Load the level file and set the offset for level rendering
    std::cout << "Level 1 Load" << endl;
    ls::LoadLevelFile("res/levels/level_1.txt", 60.0f);
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
        auto sprite = player->addComponent<SpriteComponent>(Vector2f(40.f, 60.f));
        sprite->setTexure(Resources::get<Texture>("PlayerWalkRight.png"));
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

    // Add components and sprite to goal tile
    {
        // Find the goal tile, create a new entity for it, and set position
        auto goal = ls::findTiles(ls::END)[0];
        auto g = makeEntity();
        g->addTag("goal");
        g->setPosition(ls::getTilePosition(goal) + Vector2f(30.f, 30.f));
        // Add a SpriteComponent with texture "Goal.png"
        auto sprite = g->addComponent<SpriteComponent>(defaultSize);
        sprite->setTexure(Resources::get<sf::Texture>("Goal.png"));
    }

    // Add hurt components and sprites to hazard tiles
    {
        // Find all tiles with hazard spikes pointing up and create a new entity for each
        for (auto su : ls::findTiles(ls::SPIKE_UP)) {
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
        // Add a HurtComponent with radius of 50
        blob->addComponent<HurtComponent>(50.f);
        // Add BlobComponent with a size of 50x40 and set the filter data
        blob->addComponent<BlobComponent>(Vector2f(50.f, 40.f));
        blob->getComponents<BlobComponent>()[0]->getFixture()->SetFilterData(blobFilter);
        // Add a SpriteComponent with size 50x40
        auto sprite = blob->addComponent<SpriteComponent>(Vector2f(50.f, 40.f));
        sprite->setTexure(Resources::get<Texture>("BlobGroundLeft.png"));
    }

    // Add star counter in the top left of screen
    {
        // Create a new entity
        auto starTracker = makeEntity();
        starTracker->addTag("starTracker");
        starTracker->setPosition(Vector2f(150.f, 90.f));
        // Add a SpriteComponent with texture "Star.png"
        auto sprite = starTracker->addComponent<SpriteComponent>(defaultSize);
        sprite->setTexure(Resources::get<sf::Texture>("Star.png"));
        // Add a TextComponent with the number of stars collected
        auto text = starTracker->addComponent<TextComponent>("0/3");
        text->SetColor(Color::Black);
        // Set origin to the left center of the text
        text->getText().setOrigin(Vector2f(0.f, text->getText().getLocalBounds().height * 0.5f));
        text->getText().setPosition(Vector2f(190.f, 90.f));
    }

    // Add death counter in top middle of screen
    {
        // Create a new entity and set position
        auto deathTracker = makeEntity();
        deathTracker->addTag("deathTracker");
        deathTracker->setPosition(Vector2f(210.f, 90.f));
        // Add a TextComponent with the number of total deaths on your save file
        auto text = deathTracker->addComponent<TextComponent>("Total Deaths: " +
            to_string(SaveSystem::getDeathCount()));
        text->SetColor(Color::Black);
        // Set origin to the left center of the text
        text->getText().setOrigin(Vector2f(0.f, text->getText().getLocalBounds().height * 0.5f));
        text->getText().setPosition(Vector2f(300.f, 90.f));
    }

    // Add timer display in top right of screen
    {
        // Set timer to zero
        timer = 0.f;
        // Load the previous best time from the save file
        bestTime = SaveSystem::getLevelBestTime(1);

        // Format the best time string
        if (bestTime <= 0.f)
            bestTimeString = "--:--:--";
        else
            bestTimeString = timeToString(bestTime);

        // Create an entity for the time tracker and set its position
        auto timeTracker = makeEntity();
        timeTracker->addTag("timeTracker");
        timeTracker->setPosition(Vector2f(Engine::getWindowSize().x - 180.f, 90.f));

        // Add a text component to display the current and best times
        auto text = timeTracker->addComponent<TextComponent>(
            "Best: 00:00:00 | Current: 00:00:00"
        );
        text->SetColor(Color::Black);
        // Set origin to the right center of the text
        text->getText().setOrigin(Vector2f(
            text->getText().getLocalBounds().width,
            text->getText().getLocalBounds().height * 0.5f
        ));
        // Position the text in the top right of the screen
        text->getText().setPosition(Vector2f(Engine::getWindowSize().x - 120.f, 90.f));
    }

    loadPauseMenu();

    // Set loaded flag to true
    setLoaded(true);
    // Make sure the engine is not paused
    Engine::paused = false;
}

// This function adds the given tag to the list of collected stars
void Level1Scene::AddCollected(string tag) {
    collected.push_back(tag);
}

// This function takes in a float s (in seconds) and returns a string in the format of "MM:SS:MS"
// where MM is minutes, SS is seconds, and MS is milliseconds
string Level1Scene::timeToString(float s) {
    string minutes = to_string((int)s / 60);
    // Add a leading zero if minutes is only one digit
    if (minutes.length() == 1) minutes = "0" + minutes; 
    string seconds = to_string((int)s % 60);
    // Add a leading zero if seconds is only one digit
    if (seconds.length() == 1) seconds = "0" + seconds;
    string milliseconds = to_string((int)(s * 100) % 100);
    // Add a leading zero if milliseconds is only one digit
    if (milliseconds.length() == 1) milliseconds = "0" + milliseconds;

    // Return the time string in "MM:SS:MS" format
    return minutes + ":" + seconds + ":" + milliseconds;
}

// This function creates a pause menu with a black rectangle background, 
// and menu options for resuming, restarting the level, and returning to the main menu
// It also sets the initial values for the pause menu's entities and clears the menu options vector
void Level1Scene::loadPauseMenu() {
    // Create a black rectangle background for the pause menu
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

    // Create the menu options for resuming, restarting, and exiting to the main menu
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

    // Set the selected option index to -1, and unpause the game
    selectedOptionIndex = -1;
    Engine::paused = false;
}

// This function clears the menu options vector, unloads the player, 
// blob, and level, and calls the parent Scene's unload function
void Level1Scene::Unload() {
    menuOptions.clear();
    std::cout << "Level 1 Unload" << endl;
    player.reset();
    blob.reset();
    ls::Unload();
    Scene::Unload();
}

void Level1Scene::Update(const double& dt) {
    // Only execute the following code if the game is not paused
    if (!Engine::paused) {
        // Update the UI for the number of collected stars
        ents.find("starTracker")[0]->getComponents<TextComponent>()[0]->
            SetText(to_string(collected.size()) + "/3");

        // Update the UI for the number of player deaths
        ents.find("deathTracker")[0]->getComponents<TextComponent>()[0]->
            SetText("Deaths: " + to_string(SaveSystem::getDeathCount()));

        // Update the timer for the level
        timer += dt;
        ents.find("timeTracker")[0]->getComponents<TextComponent>()[0]->
            SetText("Best: " + bestTimeString + " | Current: " + timeToString(timer));

        // Check if the player has reached the end of the level
        if (ls::getTileAt(player->getPosition()) == ls::END) {
            // Save the number of collected stars
            SaveSystem::addCollected(collected);

            // If the player's time is the best or there is no best time, save it
            if (bestTime <= 0.f || timer < bestTime) {
                bestTime = timer;
                bestTimeString = timeToString(bestTime);
                SaveSystem::addNewLevelTime(1, timer);
            }

            // Mark the level as completed and save the game
            SaveSystem::setLastLevelCompleted(1);
            SaveSystem::saveGame();
            // Transition to the end of level screen
            Engine::ChangeScene((Scene*)&endLevel);
        }
        // Check if the player is dead
        else if (!player->isAlive()) {
            // Increment the player's death counter and save the game
            SaveSystem::setDeathCounter(SaveSystem::getDeathCount() + 1);
            SaveSystem::saveGame();
            // Restart the current level
            Engine::ChangeScene((Scene*)&level1);
        }

        // Check if the player wants to exit the game
        if (Keyboard::isKeyPressed(Controls::Exit) ||
            sf::Joystick::isButtonPressed(0, 7)) {
            // Pause the game and display the menu
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
            sf::Joystick::isButtonPressed(0,0)) {
            executeSelectedOption();
        }
    }

    // update scene entities
    Scene::Update(dt);
}

void Level1Scene::Render() {
    // render level tiles
    ls::Render(Engine::getWindow());
    // render scene entities
    Scene::Render();
}

void Level1Scene::displayMenu() {
    // set menu state
    ents.find("background")[0]->setVisible(true);
    auto menu = ents.find("menu");
    for (auto menuoption : menu) {
        menuoption->setVisible(true);
        menuoption->setAlive(true);
    }
    // set player state
    ents.find("player")[0]->setAlive(false);
}

void Level1Scene::hideMenu() {
    // reset menu state
    Engine::paused = false;
    ents.find("background")[0]->setVisible(false);
    auto menu = ents.find("menu");
    for (auto menuoption : menu) {
        menuoption->setVisible(false);
        menuoption->setAlive(false);
    }
    // reset player state
    ents.find("player")[0]->setAlive(true);
}

void Level1Scene::moveUp() {
    // handle initial state when nothing is selected
    if (selectedOptionIndex - 1 >= 0) {
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::White);
        selectedOptionIndex--;
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        this_thread::sleep_for(chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
}

void Level1Scene::moveDown() {
    // handle initial state when nothing is selected
    if (selectedOptionIndex == -1) {
        selectedOptionIndex = 0;
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        this_thread::sleep_for(chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
    // handle when the last option is selected
    else if (selectedOptionIndex + 1 < menuOptions.size()) {
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::White);
        selectedOptionIndex++;
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        this_thread::sleep_for(chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
}
void Level1Scene::executeSelectedOption() {
    switch (selectedOptionIndex) {
    case 0:
        hideMenu();
        break;
    case 1:
        // restart level
        Engine::ChangeScene((Scene*)&level1);
        break;
    case 2:
        // exit to main menu
        Engine::ChangeScene(&levelMenu);
    }
}

float Level1Scene::getTimer() {
    // returns the total level time
    return timer;
}

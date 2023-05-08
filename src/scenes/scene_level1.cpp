/**
* scene_level1.cpp: implementation for Level1Scene class
*
* Author: Dillon Aitken
* Pause Menu: Julia Swietochowska
* Last modified: 04/05/2023
*/
#include "../game.h"
#include "../controls.h"
#include "../save_system.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_collectible.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_player_physics.h"

#include <thread>
#include <iostream>
#include <system_resources.h>
#include <LevelSystem.h>

#include "scene_level1.h"
#include "../components/cmp_blob.h"
#include "../components/cmp_spike_ball.h"
#include <system_physics.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player, blob;
vector<shared_ptr<Entity>> Level1Scene::menuOptions;

void Level1Scene::Load() {
    std::cout << " Scene 1 Load" << endl;
    ls::LoadLevelFile("res/levels/test_level_v3.txt", 60.0f);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 60.f);
    ls::setOffset(Vector2f(0, ho));

    // Create player
    {
        b2Filter playerFilter;
        playerFilter.categoryBits = 0x0002;
        playerFilter.maskBits = 0x0008;

        player = makeEntity();
        player->addTag("player");
        player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
        player->addComponent<PlayerPhysicsComponent>(Vector2f(50.f, 60.f));
        player->getComponents<PlayerPhysicsComponent>()[0]->getFixture()->SetFilterData(playerFilter);
        auto s = player->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(50.f, 60.f));
        s->getShape().setFillColor(Color::Magenta);
        s->getShape().setOrigin(Vector2f(25.f, 30.f));
    }

    // Create blob
    {
        b2Filter blobFilter;
        blobFilter.categoryBits = 0x0004;
        blobFilter.maskBits = 0x0008;

        blob = makeEntity();
        blob->addTag("blob");
        blob->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]) + Vector2f(30.f, 40.f));
        blob->addComponent<HurtComponent>(50.f);
        blob->addComponent<BlobComponent>(Vector2f(50.f, 40.f));
        blob->getComponents<BlobComponent>()[0]->getFixture()->SetFilterData(blobFilter);
        auto s = blob->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(50.f, 40.f));
        s->getShape().setFillColor(Color::Blue);
        s->getShape().setOrigin(Vector2f(25.f, 20.f));
    }

    // Add components and sprites to star tiles
    {
        collected.clear();

        for (string s : SaveSystem::getCollected())// for each string in SaveSystem::Collected
            if (s.rfind("Level1_", 0) == 0) // if string starts with "Level1"
                collected.push_back(s); // add string to collected

        auto stars = ls::findTiles(ls::STAR);
        for (int i = 0; i < 3; i++) {
            std::cout << "Checking for Level1_" << i + 1 << endl;
            if (find(collected.begin(), collected.end(), "Level1_" + to_string(i + 1)) != collected.end()) {
                std::cout << "Star " << i + 1 << " collected" << endl;
            } else {
                std::cout << "Star " << i + 1 << " not collected. Adding to level..." << endl;
                auto s = makeEntity();
                s->addTag("Level1_" + to_string(i + 1));
                s->setPosition(ls::getTilePosition(stars[i]) + Vector2f(30.f, 30.f));
                s->addComponent<CollectibleComponent>(55.f);
                auto sc = s->addComponent<SpriteComponent>();
                sc->setTexure(Resources::get<sf::Texture>("Star.png"));
			}
        }

        auto starTracker = makeEntity();
        starTracker->addTag("starTracker");
        starTracker->setPosition(Vector2f(150.f, 90.f));
        auto stSprite = starTracker->addComponent<SpriteComponent>();
        stSprite->setTexure(Resources::get<sf::Texture>("Star.png"));
        auto stText = starTracker->addComponent<TextComponent>("0/3");
        stText->SetColor(Color::Black);
        stText->getText().setOrigin(Vector2f(
            stText->getText().getLocalBounds().width * 0.5f,
            stText->getText().getLocalBounds().height * 0.5f
        ));
        stText->getText().setPosition(starTracker->getPosition() + Vector2f(70.f, -5.f));
    }

    // Add components and sprites to goal tile
    {
        auto goal = ls::findTiles(ls::END)[0];
        auto g = makeEntity();
        g->addTag("goal");
        g->setPosition(ls::getTilePosition(goal) + Vector2f(30.f, 30.f));
        auto sc = g->addComponent<SpriteComponent>();
        sc->setTexure(Resources::get<sf::Texture>("Goal.png"));
    }

    // Add hurt components and sprites to hazard tiles
    {
        for (auto su : ls::findTiles(ls::SPIKE_UP)){
            auto h = makeEntity();
            auto pos = ls::getTilePosition(su) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(55.f);
            auto sc = h->addComponent<SpriteComponent>();
            sc->setTexure(Resources::get<sf::Texture>("SpikeUp.png"));
        }

        for (auto sd : ls::findTiles(ls::SPIKE_DOWN)) {
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sd) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(55.f);
            auto sc = h->addComponent<SpriteComponent>();
            sc->setTexure(Resources::get<sf::Texture>("SpikeDown.png"));
        }

        for (auto sr : ls::findTiles(ls::SPIKE_RIGHT)) {
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sr) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(55.f);
            auto sc = h->addComponent<SpriteComponent>();
            sc->setTexure(Resources::get<sf::Texture>("SpikeRight.png"));
        }

        for (auto sl : ls::findTiles(ls::SPIKE_LEFT)) {
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sl) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(55.f);
            auto sc = h->addComponent<SpriteComponent>();
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
            h->addComponent<HurtComponent>(60.f);
            h->addComponent<SpikeBallComponent>(Vector2f(60.f, 60.f));
            h->getComponents<SpikeBallComponent>()[0]->getFixture()->SetFilterData(spikeBallFilter);
            auto sc = h->addComponent<SpriteComponent>();
            sc->setTexure(Resources::get<sf::Texture>("SpikeBall.png"));
        }

        for (auto sblade : ls::findTiles(ls::SAWBLADE)) {
            auto h = makeEntity();
            auto pos = ls::getTilePosition(sblade) + Vector2f(30.f, 30.f);
            h->addTag("hazard");
            h->setPosition(pos);
            h->addComponent<HurtComponent>(55.f);
            auto sc = h->addComponent<SpriteComponent>();
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

    loadPauseMenu();

    //Simulate long loading times to check loading screen works
    //this_thread::sleep_for(chrono::milliseconds(3000));
    std::cout << " Scene 1 Load Done" << endl;

    setLoaded(true);
}

void Level1Scene::AddCollected(string tag) {
    collected.push_back(tag);
}

void Level1Scene::loadPauseMenu() {
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

void Level1Scene::Unload() {
    menuOptions.clear();
    std::cout << "Scene 1 Unload" << endl;
    player.reset();
    blob.reset();
    ls::Unload();
    Scene::Unload();
}

void Level1Scene::Update(const double& dt) {
    if (!Engine::paused) {
        ents.find("starTracker")[0]->getComponents<TextComponent>()[0]->
            SetText(to_string(collected.size()) + "/3");

        if (ls::getTileAt(player->getPosition()) == ls::END) {
            SaveSystem::addCollected(collected);
            SaveSystem::setLastLevelCompleted(1);
            SaveSystem::setDeathCounter(10);
            SaveSystem::saveGame();
            Engine::ChangeScene((Scene*)&endScene);
        }
        else if (!player->isAlive()) {
            SaveSystem::setDeathCounter(SaveSystem::getDeathCount() + 1);
            Engine::ChangeScene((Scene*)&level1);
        }

        if (Keyboard::isKeyPressed(Controls::Exit)) {
            Engine::paused = true;
            displayMenu();
        }
    }
    // pause menu update
    else {
        if (Keyboard::isKeyPressed(Controls::MenuDown)) {
            moveDown();
        }
        if (Keyboard::isKeyPressed(Controls::MenuUp)) {
            moveUp();
        }
        if (Keyboard::isKeyPressed(Controls::MenuSelect)) {
            executeSelectedOption();
        }
    }
    Scene::Update(dt);
}

void Level1Scene::Render() {
    ls::Render(Engine::getWindow());
    Scene::Render();
}

void Level1Scene::displayMenu() {
    ents.find("background")[0]->setVisible(true);
    auto menu = ents.find("menu");
    for (auto menuoption : menu) {
        menuoption->setVisible(true);
        menuoption->setAlive(true);
    }
    ents.find("player")[0]->setAlive(false);
}

void Level1Scene::hideMenu() {
    Engine::paused = false;
    ents.find("background")[0]->setVisible(false);
    auto menu = ents.find("menu");
    for (auto menuoption : menu) {
        menuoption->setVisible(false);
        menuoption->setAlive(false);
    }
    ents.find("player")[0]->setAlive(true);
}

void Level1Scene::moveUp() {
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
        Engine::ChangeScene((Scene*)&level1);
        break;
    case 2:
        Engine::ChangeScene(&menu);
    }
}

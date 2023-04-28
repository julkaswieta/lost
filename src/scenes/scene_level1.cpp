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

    // Add physics colliders to level tiles
    {
        auto walls = ls::findTiles(ls::WALL);
        for (auto w : walls) {
            auto pos = ls::getTilePosition(w);
            pos += Vector2f(30.f, 30.f); //offset to center
            auto e = makeEntity();
            e->addTag("wall");

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
            h->addTag("hazard");
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

    loadPauseMenu();

    //Simulate long loading times to check loading screen works
    //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    cout << " Scene 1 Load Done" << endl;

    setLoaded(true);
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

    string optionsText[4] = { "Game Paused", "Resume", "Save", "Exit to Main Menu" };
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
    cout << "Scene 1 Unload" << endl;
    player.reset();
    ls::Unload();
    Scene::Unload();
}

void Level1Scene::Update(const double& dt) {
    if (!Engine::paused) {
        if (ls::getTileAt(player->getPosition()) == ls::END) {
            Engine::ChangeScene((Scene*)&menu);
        }
        else if (!player->isAlive())
        {
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
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
}

void Level1Scene::moveDown() {
    // handle initial state when nothing is selected
    if (selectedOptionIndex == -1) {
        selectedOptionIndex = 0;
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
    else if (selectedOptionIndex + 1 < menuOptions.size()) {
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::White);
        selectedOptionIndex++;
        menuOptions[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
}
void Level1Scene::executeSelectedOption() {
    switch (selectedOptionIndex) {
    case 0:
        hideMenu();
        break;
    case 1:
        //save
        break;
    case 2:
        Engine::ChangeScene(&menu);
    }
}

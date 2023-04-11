#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

vector<shared_ptr<Entity>> options;

void MenuScene::Load() {
    cout << "Menu Load \n";
    {
        string optionsText[4] = { "LOST", "Start Game", "Settings", "Exit" };
        for (int i = 0; i < 4; ++i) {
            auto menuOption = makeEntity();
            auto textCmp = menuOption->addComponent<TextComponent>(optionsText[i]);
            textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
            textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, TOP_MARGIN + ((i + 1) * 50)));
            if(i > 0)
                options.push_back(menuOption);
        }
    }
    selectedOptionIndex = -1;
    setLoaded(true);
}

void MenuScene::Update(const double& dt) {
    // cout << "Menu Update "<<dt<<"\n";
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        moveDown();
        
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        moveUp();
        
    }
    if (Keyboard::isKeyPressed(Keyboard::Enter)) {
        executeSelectedOption();
    }

    Scene::Update(dt);
}

void MenuScene::moveUp() {
    if (selectedOptionIndex - 1 >= 0) {
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->getText().setFillColor(Color::White);
        selectedOptionIndex--;
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->getText().setFillColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(120)); // these are here so the cursor does not move too fast
    }
}

void MenuScene::moveDown() {
    // handle initial state when nothing is selected
    if (selectedOptionIndex == -1) {
        selectedOptionIndex = 0;
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->getText().setFillColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(120)); // these are here so the cursor does not move too fast
    }
    else if (selectedOptionIndex + 1 < OPTIONS_COUNT) {
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->getText().setFillColor(Color::White);
        selectedOptionIndex++;
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->getText().setFillColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(120)); // these are here so the cursor does not move too fast
    }
}

void MenuScene::executeSelectedOption() {
    switch (selectedOptionIndex) {
    case 0:
        Engine::ChangeScene(&level1);
        break;
    case 1:
        //Engine::ChangeScene(&settings);
        break;
    case 2:
        Engine::getWindow().close();
    }
}

/**
* scene_menu.cpp: implementation for MenuScene class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#include "scene_menu.h"

#include "../controls.h"
#include "../components/cmp_text.h"
#include "engine.h"

using namespace std;
using namespace sf;

std::vector<std::shared_ptr<Entity>> MenuScene::options;

void MenuScene::Unload()
{
    options.clear();
    Scene::Unload();
}

void MenuScene::Update(const double& dt)
{
    if (Keyboard::isKeyPressed(Controls::MenuDown) || sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) <= -20) {
        moveDown();
    }
    if (Keyboard::isKeyPressed(Controls::MenuUp) || sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) >= 20) {
        moveUp();
    }
    if (Keyboard::isKeyPressed(Controls::MenuSelect) || sf::Joystick::isButtonPressed(0,0)) {
        executeSelectedOption();
    }
     Scene::Update(dt);
}

void MenuScene::moveUp() {
    if (selectedOptionIndex - 1 >= 0) {
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::White);
        selectedOptionIndex--;
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
}

void MenuScene::moveDown() {
    // handle initial state when nothing is selected
    if (selectedOptionIndex == -1) {
        selectedOptionIndex = 0;
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
    else if (selectedOptionIndex + 1 < options.size()) {
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::White);
        selectedOptionIndex++;
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
}

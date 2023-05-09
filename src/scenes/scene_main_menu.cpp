/**
* scene_main_menu.cpp: implementation for MainMenuScene class
*
* Author: Julia Swietochowska
* Last modified: 09/05/2023
*/
#include "scene_main_menu.h"

#include <iostream>

#include "../game.h"
#include "../controls.h"
#include "../save_system.h"
#include "../components/cmp_text.h"

using namespace std;
using namespace sf;

void MainMenuScene::Load() {
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

void MainMenuScene::executeSelectedOption() {
    switch (selectedOptionIndex) {
    case 0:
        Engine::ChangeScene(&level1);
        break;
    case 1:
        Engine::ChangeScene(&settings);
        break;
    case 2:
        Engine::getWindow().close();
    }
}

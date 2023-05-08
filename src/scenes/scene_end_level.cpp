/**
* scene_end_level.cpp: implementation for EndLevelScene class
*
* Author: Julia Swietochowska
* Last modified: 08/05/2023
*/
#include "scene_end_level.h"
#include "../controls.h"
#include "engine.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include "../save_system.h"

using namespace std;
using namespace sf;

void EndLevelScene::Load()
{
    string optionsText[7] = { "Level Completed!", "Retry level", "Next level", "Exit to Main Menu", "Deaths so far:", "Level time:", "Stars collected" };
    for (int i = 0; i < 7; ++i) {
        auto menuOption = makeEntity();
        auto textCmp = menuOption->addComponent<TextComponent>(optionsText[i]);
        textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
        textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, 200 + ((i + 1) * 50)));
        menuOption->addTag(optionsText[i]);
        if (i > 0 && i < 4)
            options.push_back(menuOption);
        if (i >= 4) {
            textCmp->SetColor(Color::Blue);
            textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, 300 + ((i + 1) * 50)));
        }
    }
    previousLevel = SaveSystem::getLastLevelCompleted();
    deathCount = SaveSystem::getDeathCount();
} 

void EndLevelScene::Unload()
{
    options.clear();
    MenuScene::Unload();
}

void EndLevelScene::Update(const double& dt)
{
    ents.find("Deaths so far:")[0]->getComponents<TextComponent>()[0]->SetText("Deaths so far: " + to_string(deathCount));
    MenuScene::Update(dt);
}

void EndLevelScene::executeSelectedOption()
{    
    switch (selectedOptionIndex) {
    case 0:
        Engine::ChangeScene(levels[previousLevel - 1]);
        break;
    case 1:
        Engine::ChangeScene(levels[previousLevel]);
        break;
    case 2:
        Engine::ChangeScene(&menu);
        break;
    }
}

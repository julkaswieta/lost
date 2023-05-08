/**
* scene_end.cpp: implementation for EndScene class
*
* Author: Julia Swietochowska
* Last modified: 08/05/2023
*/
#include "scene_end.h"
#include "../controls.h"
#include "engine.h"
#include "../game.h"
#include "../components/cmp_text.h"

using namespace std;
using namespace sf;

void EndScene::Load()
{
	string optionsText[3] = { "Congratulations!", "Player is now out of Void and reunited with their dog!", "Press Enter to go back to Main Menu" };
    for (int i = 0; i < 3; ++i) {
        auto menuOption = makeEntity();
        auto textCmp = menuOption->addComponent<TextComponent>(optionsText[i]);
        textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
        textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, 200 + ((i + 1) * 50)));
        if (i == 2)
            textCmp->SetColor(Color::Blue);
    }
}

void EndScene::Update(const double& dt)
{
	if (Keyboard::isKeyPressed(Controls::MenuSelect)) {
		Engine::ChangeScene(&menu);
	}
	Scene::Update(dt);
}

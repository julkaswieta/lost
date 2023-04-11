#include "scene_settings.h"
#include "../components/cmp_text.h"
#include "engine.h"
#include "SFML/Window/Keyboard.hpp"
#include "../game.h"
#include "../controls.h"

using namespace std;
using namespace sf;

void SettingsScene::Load() {
    {
        string optionsText[5] = { "Settings", "Volume", "Controls", "Resolution", "Exit"};
        for (int i = 0; i < 5; ++i) {
            auto menuOption = makeEntity();
            auto textCmp = menuOption->addComponent<TextComponent>(optionsText[i]);
            textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
            textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, TOP_MARGIN + ((i + 1) * 50)));
            if (i > 0)
                options.push_back(menuOption);
        }
    }
    selectedOptionIndex = -1;
    setLoaded(true);

}

void SettingsScene::Update(const double& dt) {
    if (Keyboard::isKeyPressed(Controls::MenuDown)) {
        moveDown();
    }
    if (Keyboard::isKeyPressed(Controls::MenuDown)) {
        moveUp();
    }
    if (Keyboard::isKeyPressed(Controls::MenuSelect)) {
        executeSelectedOption();
    }

    Scene::Update(dt);
}

void SettingsScene::moveUp() {
    if (selectedOptionIndex - 1 >= 0) {
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::White);
        selectedOptionIndex--;
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
}

void SettingsScene::moveDown() {
    // handle initial state when nothing is selected
    if (selectedOptionIndex == -1) {
        selectedOptionIndex = 0;
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
    else if (selectedOptionIndex + 1 < OPTIONS_COUNT) {
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::White);
        selectedOptionIndex++;
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
}

void SettingsScene::executeSelectedOption() {
    switch (selectedOptionIndex) {
    case 0:
        //Engine::ChangeScene(&volume);
        break;
    case 1:
        //Engine::ChangeScene(&controls);
        break;
    case 2:
        //Engine::ChangeScene(&resolution);
        break;
    case 3:
        Engine::ChangeScene(&menu);
        break;
    }
}

void SettingsScene::Unload() {
    options.clear();
    Scene::Unload();
}

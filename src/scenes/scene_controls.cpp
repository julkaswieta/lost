#include "scene_controls.h"
#include "../controls.h"
#include "../components/cmp_text.h"
#include "engine.h"
#include "../game.h"

using namespace std;
using namespace sf;

void ControlsScene::Load() {
    {
        const int elementsCount = 14;
        string optionsText[elementsCount] = { "Controls", "Keyboard", "Controller", 
            "Gameplay Controls", "Move Left", "Move Right", "Jump", "Pause", 
            "Menu Navigation", "Move Up", "Move Down", "Select Option", 
            "Restore Defaults", "Exit"};
        for (int i = 0; i < elementsCount; ++i) {
            auto menuOption = makeEntity();
            auto textCmp = menuOption->addComponent<TextComponent>(optionsText[i]);
            textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
            textCmp->getText().setPosition(positionOptionInWindow(i));
            if (i != 0 && i != 3 && (i < 8 || i > 11)) // push only the elements with actions to options (menu navigation currently not remappable)
                options.push_back(menuOption);
        }
    }
    selectedOptionIndex = -1;
    setLoaded(true);
}

Vector2f ControlsScene::positionOptionInWindow(int i) {
    switch (i) {
    // top centered options
    case 0:
        return Vector2f(Engine::getWindowSize().x * 0.5f, TOP_MARGIN + ((i + 1) * 50));
    case 1:
        return Vector2f(Engine::getWindowSize().x * 0.5f - LEFT_MARGIN, TOP_MARGIN + ((i + 1) * 50));
    case 2:
        return Vector2f(Engine::getWindowSize().x * 0.5f + LEFT_MARGIN, TOP_MARGIN + ((i) * 50));
    // game controls
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        return Vector2f(LEFT_MARGIN, TOP_MARGIN + ((i + 1) * 50));
    // menu navigation
    case 8:
    case 9:
    case 10:
    case 11:
        return Vector2f(Engine::getWindowSize().x - LEFT_MARGIN, TOP_MARGIN + ((i - 4) * 50));
    //bottom centered options
    case 12:
    case 13:
        return Vector2f(Engine::getWindowSize().x * 0.5f, TOP_MARGIN + ((i - 3) * 50));
    }
}

void ControlsScene::Unload() {
    options.clear();
    Scene::Unload();
}

void ControlsScene::Update(const double& dt) {
    if (Keyboard::isKeyPressed(Controls::MenuDown)) {
        moveDown();
    }
    if (Keyboard::isKeyPressed(Controls::MenuUp)) {
        moveUp();
    }
    if (Keyboard::isKeyPressed(Controls::MenuSelect)) {
        executeSelectedOption();
    }

    Scene::Update(dt);
}

void ControlsScene::moveUp() {
    if (selectedOptionIndex - 1 >= 0) {
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::White);
        selectedOptionIndex--;
        options[selectedOptionIndex]->getComponents<TextComponent>()[0]->SetColor(Color::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); // these are here so the cursor does not move too fast
    }
}

void ControlsScene::moveDown() {
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

void ControlsScene::executeSelectedOption() {
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
        Engine::ChangeScene(&settings);
        break;
    }
}

#include "scene_controls.h"
#include "../controls.h"
#include "../components/cmp_text.h"
#include "engine.h"
#include "../game.h"
#include "SFML/Window/Event.hpp"
#include "system_renderer.h"

using namespace std;
using namespace sf;

void ControlsScene::Load() {
    {
        const int elementsCount = 14;
        string optionsText[elementsCount] = { "Controls", "Keyboard", "Controller",
            "Gameplay Controls", "Move Left", "Move Right", "Jump", "Pause",
            "Menu Navigation", "Move Up", "Move Down", "Select Option",
            "Restore Defaults", "Exit" };
        for (int i = 0; i < elementsCount; ++i) {
            auto menuOption = makeEntity();
            auto textCmp = menuOption->addComponent<TextComponent>(optionsText[i]);
            menuOption->addTag(optionsText[i]);
            textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
            textCmp->getText().setPosition(positionOptionInWindow(i));
            if (i != 0 && i != 3 && (i < 7 || i > 11)) // push only the elements with actions to options (menu navigation currently not remappable)
                options.push_back(menuOption);
        }

        {
            string remapText = "Press Enter to remap this control, then press the new key";

            auto remapMessage = makeEntity();
            auto textCmp = remapMessage->addComponent<TextComponent>(remapText);
            remapMessage->setVisible(false);
            remapMessage->addTag("remap");
            textCmp->SetColor(Color::Blue);
            textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
            textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, Engine::getWindowSize().y - textCmp->getText().getLocalBounds().height));
        }

    }
    ACTIVE_OPTIONS_COUNT = options.size();
    selectedOptionIndex = -1;
    setLoaded(true);
}

Vector2f ControlsScene::positionOptionInWindow(int i) {
    switch (i) {
        // top centered options
    case 0:
        return Vector2f(Engine::getWindowSize().x * 0.5f, TOP_MARGIN + ((i + 1) * SPACING));
    case 1:
        return Vector2f(Engine::getWindowSize().x * 0.5f - LEFT_MARGIN, TOP_MARGIN + ((i + 1) * SPACING));
    case 2:
        return Vector2f(Engine::getWindowSize().x * 0.5f + LEFT_MARGIN, TOP_MARGIN + ((i)*SPACING));
        // game controls
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        return Vector2f(LEFT_MARGIN, TOP_MARGIN + ((i + 1) * SPACING));
        // menu navigation
    case 8:
    case 9:
    case 10:
    case 11:
        return Vector2f(Engine::getWindowSize().x - LEFT_MARGIN, TOP_MARGIN + ((i - 4) * SPACING));
        //bottom centered options
    case 12:
    case 13:
        return Vector2f(Engine::getWindowSize().x * 0.5f, TOP_MARGIN + ((i - 3) * SPACING));
    }
}

void ControlsScene::Update(const double& dt) {
    updateControlsUI();
    MenuScene::Update(dt);
}

void ControlsScene::updateControlsUI() {
    this->ents.find("Move Left")[0]->getComponents<TextComponent>()[0]->SetText("Move Left: " + Controls::toString(Controls::MoveLeft));
    this->ents.find("Move Right")[0]->getComponents<TextComponent>()[0]->SetText("Move Right: " + Controls::toString(Controls::MoveRight));
    this->ents.find("Jump")[0]->getComponents<TextComponent>()[0]->SetText("Jump: " + Controls::toString(Controls::Jump));
    this->ents.find("Pause")[0]->getComponents<TextComponent>()[0]->SetText("Pause: " + Controls::toString(Controls::Exit));
    this->ents.find("Move Up")[0]->getComponents<TextComponent>()[0]->SetText("Jump: " + Controls::toString(Controls::MenuUp));
    this->ents.find("Move Down")[0]->getComponents<TextComponent>()[0]->SetText("Jump: " + Controls::toString(Controls::MenuDown));
    this->ents.find("Select Option")[0]->getComponents<TextComponent>()[0]->SetText("Jump: " + Controls::toString(Controls::MenuSelect));
}

void ControlsScene::moveUp() {
    MenuScene::moveUp();
    if (selectedOptionIndex >= 2 && selectedOptionIndex <= 4) {
        this->ents.find("remap")[0]->setVisible(true);
    }
    else {
        this->ents.find("remap")[0]->setVisible(false);
    }
}

void ControlsScene::moveDown() {
    MenuScene::moveDown();
    if (selectedOptionIndex >= 2 && selectedOptionIndex <= 4) {
        this->ents.find("remap")[0]->setVisible(true);
    }
    else {
        this->ents.find("remap")[0]->setVisible(false);
    }
}

void ControlsScene::executeSelectedOption() {
    RenderWindow& rw = Engine::getWindow();
    bool optionExecuted = false;

    while (!optionExecuted)
    {
        switch (selectedOptionIndex) {
        case 0:
            //Keyboard - display keyboard bindings and set keyboard as the play mode
            optionExecuted = true;
            break;
        case 1:
            //Controller - display controller bindings and set controller as play mode
            optionExecuted = true;
            break;
        case 2:
        {
            this->ents.find("Move Left")[0]->getComponents<TextComponent>()[0]->SetColor(Color::Blue);
            // Move left rebind
            Event event;
            while (rw.pollEvent(event)) {
                if (event.type == sf::Event::EventType::KeyPressed) {
                    if (event.key.code != 58) {
                        Controls::MoveLeft = event.key.code;
                        optionExecuted = true;
                        break;
                    }
                }
            }
        }
        break;
        case 3:
        {
            this->ents.find("Move Right")[0]->getComponents<TextComponent>()[0]->SetColor(Color::Blue);
            // Move right rebind
            Event event;
            while (rw.pollEvent(event)) {
                if (event.type == sf::Event::EventType::KeyPressed) {
                    if (event.key.code != 58) {
                        Controls::MoveRight = event.key.code;
                        optionExecuted = true;
                        break;
                    }
                }
            }
        }
        break;
        case 4:
        {
            this->ents.find("Jump")[0]->getComponents<TextComponent>()[0]->SetColor(Color::Blue);
            // Jump rebind
            Event event;
            while (rw.pollEvent(event)) {
                if (event.type == sf::Event::EventType::KeyPressed) {
                    if (event.key.code != 58) {
                        Controls::Jump = event.key.code;
                        optionExecuted = true;
                        break;
                    }
                }
            }
        }
        break;
        case 5:
            // restore defaults
            restoreDefaults();
            optionExecuted = true;
            break;
        case 6:
            optionExecuted = true;
            Engine::ChangeScene(&settings);
            break;
        default:
            optionExecuted = true;
            break;
        }
    }
}

void ControlsScene::restoreDefaults() {
    Controls::MoveLeft = Keyboard::Left;
    Controls::MoveRight = Keyboard::Right;
    Controls::Jump = Keyboard::Up;
}
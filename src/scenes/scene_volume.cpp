/**
* scene_volume.cpp: implementation for VolumeScene class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#include "scene_volume.h"

#include "../game.h"
#include "../controls.h"
#include "../save_system.h"
#include "../components/cmp_text.h"
#include "engine.h"

using namespace std;
using namespace sf;

void VolumeScene::Load()
{
    {
        string optionsText[5] = { "Volume Settings", "Change Volume", "Volume", "Exit", "Use left and right arrows to change volume" };
        for (int i = 0; i < 5; ++i) {
            auto menuOption = makeEntity();
            auto textCmp = menuOption->addComponent<TextComponent>(optionsText[i]);
            textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
            if (i != 4) {
                textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, TOP_MARGIN + ((i + 1) * 50)));
                menuOption->addTag(optionsText[i]);
            }
            else {
                textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, Engine::getWindowSize().y - TOP_MARGIN));
                textCmp->SetColor(Color::Blue);
                menuOption->addTag("volMessage");
                menuOption->setVisible(false);
            }
            if (i == 1 || i == 3)
                options.push_back(menuOption);
        }
    }
    localVolume = SaveSystem::getVolume();
    selectedOptionIndex = -1;
    setLoaded(true);
}

void VolumeScene::Update(const double& dt)
{
    this->ents.find("Volume")[0]->getComponents<TextComponent>()[0]->SetText("Volume: " + to_string(localVolume));

    if (volumeChangeActive) {
        if (Keyboard::isKeyPressed(Controls::NextOption) || sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) >= 20) {
            volumeUp();
        }
        if (Keyboard::isKeyPressed(Controls::PreviousOption) || sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) <= -20) {
            volumeDown();
        }
    }

    MenuScene::Update(dt);
}

void VolumeScene::moveUp()
{
    resetFormatting();
    MenuScene::moveUp();
}

void VolumeScene::moveDown()
{
    resetFormatting();
    MenuScene::moveDown();
}

void VolumeScene::volumeUp() {
    if (localVolume < 100) {
        localVolume++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void VolumeScene::volumeDown() {
    if (localVolume > 0) {
        localVolume--;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void VolumeScene::resetFormatting()
{
    volumeChangeActive = false;
    this->ents.find("Volume")[0]->getComponents<TextComponent>()[0]->SetColor(Color::White);
    this->ents.find("volMessage")[0]->setVisible(false);
}

void VolumeScene::executeSelectedOption()
{
    switch (selectedOptionIndex) {
    case 0:
        volumeChangeActive = true;
        this->ents.find("Volume")[0]->setVisible(true);
        this->ents.find("Volume")[0]->getComponents<TextComponent>()[0]->SetColor(Color::Blue);
        this->ents.find("volMessage")[0]->setVisible(true);
        break;
    case 1:
        SaveSystem::updateVolume(localVolume);
        SaveSystem::saveSettings();
        Engine::ChangeScene(&settings);
        break;
    }
}

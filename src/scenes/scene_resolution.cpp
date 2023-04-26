#include "scene_resolution.h"
#include "../components/cmp_text.h"
#include "engine.h"
#include "SFML/Window/Keyboard.hpp"
#include "../game.h"

using namespace std;
using namespace sf;

void ResolutionScene::Load() {
    string optionsText[5] = { "Resolution", "Change Resolution", "Change window display mode", "Exit", "Use left and right arrows to change this setting" };
    string resolutionsText[6] = { "640 x 360", "1280 x 720", "1600 x 900", "1920 x 1080", "2048 x 1152", "2560 x 1440" };
    string windowModeText[2] = { "Fullscreen", "Windowed" };

    // general menu options
    {
        for (int i = 0; i < 5; ++i) {
            auto menuOption = makeEntity();
            auto textCmp = menuOption->addComponent<TextComponent>(optionsText[i]);
            textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
            menuOption->addTag("option" + to_string(i));
            if (i != 4) {
                textCmp->getText().setPosition(positionElement(i));
            }
            else {
                textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, Engine::getWindowSize().y - TOP_MARGIN));
                textCmp->SetColor(Color::Blue);
                menuOption->addTag("changeMessage");
                menuOption->setVisible(false);
            }
            if (i > 0 && i < 4)
                options.push_back(menuOption);
        }
    }

    // resolutions 
    {
        for (int i = 0; i < 6; ++i) {
            auto resolutionOption = makeEntity();
            auto textCmp = resolutionOption->addComponent<TextComponent>(resolutionsText[i]);
            textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
            resolutionOption->addTag(resolutionsText[i]);
            textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, 250));
            //textCmp->SetColor(Color::Red);
            resolutionOption->setVisible(false);
            resolutions.push_back(resolutionOption);
        }
    }

    // window modes
    {
        for (int i = 0; i < 2; ++i) {
            auto windowMode = makeEntity();
            auto textCmp = windowMode->addComponent<TextComponent>(windowModeText[i]);
            textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
            windowMode->addTag("window" + to_string(i));
            textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, 350));
            //textCmp->SetColor(Color::Red);
            windowMode->setVisible(false);
            windowModes.push_back(windowMode);
        }
    }

    ACTIVE_OPTIONS_COUNT = options.size();
    currentResolutionOption = resolutionsText[3];
    resolution = resolutionToVector(currentResolutionOption);
    currentWindowMode = 1;
    resolutionChangeActive = false;
    windowModeChangeActive = false;
    selectedOptionIndex = -1;
    setLoaded(true);
}

Vector2f ResolutionScene::positionElement(int elemIndex) {
    switch (elemIndex) {
    case 0:
    case 1:
        return Vector2f(Engine::getWindowSize().x * 0.5f, TOP_MARGIN + ((elemIndex + 1) * 50));
    case 2:
        return Vector2f(Engine::getWindowSize().x * 0.5f, TOP_MARGIN + ((elemIndex + 2) * 50));
    case 3:
        return Vector2f(Engine::getWindowSize().x * 0.5f, TOP_MARGIN + ((elemIndex + 3) * 50));
    case 4:
        return Vector2f(Engine::getWindowSize().x * 0.5f, Engine::getWindowSize().y - TOP_MARGIN);
    }
}

void ResolutionScene::Update(const double& dt)
{
    displayCurrentSettings();
    MenuScene::Update(dt);
}

void ResolutionScene::moveUp()
{
    MenuScene::moveUp();
}

void ResolutionScene::moveDown()
{
    MenuScene::moveDown();
}

void ResolutionScene::nextResolution()
{
    
}

void ResolutionScene::executeSelectedOption()
{
    switch (selectedOptionIndex) {
    case 0:
        // change resolution
        break;
    case 1:
        // change window mode
        break;
    case 2:
        Engine::ChangeScene(&settings);
        break;
    }

}

sf::Vector2f ResolutionScene::resolutionToVector(std::string resolutionText) {
    char delimiter = 'x';
    string token;
    stringstream ss(resolutionText);
    int splitResolution[2];
    getline(ss, token, delimiter);
    splitResolution[0] = stoi(token);
    getline(ss, token, delimiter);
    splitResolution[1] = stoi(token);
    return sf::Vector2f(splitResolution[0], splitResolution[1]);
}

void ResolutionScene::displayCurrentSettings()
{
    ents.find(currentResolutionOption)[0]->setVisible(true);
    ents.find("window" + to_string(currentWindowMode))[0]->setVisible(true);
}





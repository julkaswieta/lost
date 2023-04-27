#include "scene_resolution.h"
#include "../components/cmp_text.h"
#include "engine.h"
#include "SFML/Window/Keyboard.hpp"
#include "../game.h"
#include "../controls.h"
#include "SFML/Window/VideoMode.hpp"

using namespace std;
using namespace sf;

void ResolutionScene::Load() {
    
    loadOptions();
    loadResolutions();
    loadWindowModes();

    setLoaded(true);
}

void ResolutionScene::loadOptions() {
    string optionsText[5] = { "Resolution", "Change Resolution", "Change window display mode", "Exit", "Use left and right arrows to change this setting" };
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
    ACTIVE_OPTIONS_COUNT = options.size();
    selectedOptionIndex = -1;
}

void ResolutionScene::loadResolutions() {
    string resolutionsText[5] = { "1280 x 720", "1600 x 900", "1920 x 1080", "2048 x 1152", "2560 x 1440" };
    for (int i = 0; i < 5; ++i) {
        auto resolutionOption = makeEntity();
        auto textCmp = resolutionOption->addComponent<TextComponent>(resolutionsText[i]);
        textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
        resolutionOption->addTag(resolutionsText[i]);
        textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, 250));
        //textCmp->SetColor(Color::Red);
        resolutionOption->setVisible(false);
        resolutions.push_back(resolutionOption);
    }

    if (resolution.x == 0 && resolution.y == 0) {
        resolutionCounter = 2;
        currentResolutionOption = resolutionsText[2];
        resolution = resolutionToVector(currentResolutionOption);
    }
    resolutionChangeActive = false;
}

void ResolutionScene::loadWindowModes() {
    string windowModeText[2] = { "Fullscreen", "Windowed" };
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
    
    if(currentWindowMode == -1)
        currentWindowMode = 0;
    windowModeChangeActive = false;
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

void ResolutionScene::Update(const double& dt) {
    displayCurrentSettings();
    if (resolutionChangeActive) {
        if (Keyboard::isKeyPressed(Controls::NextOption)) {
            nextResolution(true);
        }
        if (Keyboard::isKeyPressed(Controls::PreviousOption)) {
            nextResolution(false);
        }
    }

    if (windowModeChangeActive) {
        if (Keyboard::isKeyPressed(Controls::NextOption)) {
            nextWindowMode(true);
        }
        if (Keyboard::isKeyPressed(Controls::PreviousOption)) {
            nextWindowMode(false);
        }
    }

    MenuScene::Update(dt);
}

inline void ResolutionScene::displayCurrentSettings() {
    if (resolutionChangeActive)
        ents.find(currentResolutionOption)[0]->getComponents<TextComponent>()[0]->SetColor(Color::Blue);
    if (windowModeChangeActive)
        windowModes[currentWindowMode]->getComponents<TextComponent>()[0]->SetColor(Color::Blue);
    ents.find(currentResolutionOption)[0]->setVisible(true);
    ents.find("window" + to_string(currentWindowMode))[0]->setVisible(true);
}

void ResolutionScene::moveUp() {
    changeSettings();
    resetFormatting();
    MenuScene::moveUp();
}

void ResolutionScene::moveDown() {
    changeSettings();
    resetFormatting();
    MenuScene::moveDown();
}

void ResolutionScene::resetFormatting() {
    resolutionChangeActive = false;
    ents.find(currentResolutionOption)[0]->getComponents<TextComponent>()[0]->SetColor(Color::White);
    windowModeChangeActive = false;
    ents.find("window" + to_string(currentWindowMode))[0]->getComponents<TextComponent>()[0]->SetColor(Color::White);
    ents.find("changeMessage")[0]->setVisible(false);
}

void ResolutionScene::changeSettings() {
    if (resolutionChangeActive || windowModeChangeActive) {
        Engine::getWindow().create(VideoMode({ resolution.x, resolution.y }), Engine::gameName, (currentWindowMode == 0) ? Style::Fullscreen : Style::Default);
        updateElementsPosition();
    }
}

void ResolutionScene::updateElementsPosition() {
    float windowWidth = Engine::getWindowSize().x;
    for (auto ent : this->ents.list) {
        auto textCmp = ent->getComponents<TextComponent>()[0];
        textCmp->getText().setPosition({ windowWidth * 0.5f, textCmp->getText().getPosition().y });
    }
}

void ResolutionScene::nextResolution(bool moveUp)
{
    resolutions[resolutionCounter]->setVisible(false);
    if (moveUp) 
        resolutionCounter++;
    else
        resolutionCounter--;

    if (resolutionCounter < 0) 
        resolutionCounter = resolutions.size() - 1;
    
    else if (resolutionCounter >= resolutions.size()) 
        resolutionCounter = 0;
    
    currentResolutionOption = resolutions[resolutionCounter]->getComponents<TextComponent>()[0]->getText().getString();
    resolution = resolutionToVector(currentResolutionOption);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    
}

void ResolutionScene::nextWindowMode(bool moveUp)
{
    windowModes[currentWindowMode]->setVisible(false);
    if (moveUp)
        currentWindowMode++;
    else
        currentWindowMode--;

    if (currentWindowMode < 0)
        currentWindowMode = windowModes.size() - 1;

    else if (currentWindowMode >= windowModes.size())
        currentWindowMode = 0;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}

void ResolutionScene::executeSelectedOption()
{
    switch (selectedOptionIndex) {
    case 0:
        // change resolution
        resolutionChangeActive = true;
        ents.find(currentResolutionOption)[0]->getComponents<TextComponent>()[0]->SetColor(Color::Blue);
        ents.find("changeMessage")[0]->setVisible(true);
        break;
    case 1:
        // change window mode
        windowModeChangeActive = true;
        ents.find("window" + to_string(currentWindowMode))[0]->getComponents<TextComponent>()[0]->SetColor(Color::Blue);
        ents.find("changeMessage")[0]->setVisible(true);
        break;
    case 2:
        Engine::ChangeScene(&settings);
        break;
    }

}

sf::Vector2u ResolutionScene::resolutionToVector(std::string resolutionText) {
    char delimiter = 'x';
    string token;
    stringstream ss(resolutionText);
    int splitResolution[2];
    getline(ss, token, delimiter);
    splitResolution[0] = stoi(token);
    getline(ss, token, delimiter);
    splitResolution[1] = stoi(token);
    return sf::Vector2u(splitResolution[0], splitResolution[1]);
}

void ResolutionScene::Unload() {
    resolutions.clear();
    windowModes.clear();
    MenuScene::Unload();
}
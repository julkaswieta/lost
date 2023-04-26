#include "scene_resolution.h"
#include "../components/cmp_text.h"
#include "engine.h"

using namespace std;
using namespace sf;

void ResolutionScene::Load() {
    {
        string optionsText[5] = { "Resolution", "Change Resolution", "Change window display mode", "Exit", "Use left and right arrows to change volume" };
        string resolutionsText[6] = { "640 x 360", "1280 x 720", "1600 x 900", "1920 x 1080", "2048 x 1152", "2560 x 1440" };
        string windowModeText[2] = { "Fullscreen", "Windowed" };

        for (int i = 0; i < 5; ++i) {
            auto menuOption = makeEntity();
            auto textCmp = menuOption->addComponent<TextComponent>(optionsText[i]);
            textCmp->getText().setOrigin(Vector2(textCmp->getText().getLocalBounds().width * 0.5f, textCmp->getText().getLocalBounds().height * 0.5f));
            menuOption->addTag("option" + to_string(i));
            // TODO: position elements
            if (i != 4) {
                textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, TOP_MARGIN + ((i + 1) * 50)));
            }
            else {
                textCmp->getText().setPosition(Vector2f(Engine::getWindowSize().x * 0.5f, Engine::getWindowSize().y - TOP_MARGIN));
                textCmp->SetColor(Color::Blue);
                menuOption->addTag("volMessage");
                menuOption->setVisible(false);
            }
            if (i > 0 && i < 4)
                options.push_back(menuOption);
        }
    }
}

Vector2f ResolutionScene::positionElement(int elemIndex) {
    switch (elemIndex) {

    }
    return { 1,2 };
}

void ResolutionScene::Update(const double& dt)
{
}

void ResolutionScene::moveUp()
{
}

void ResolutionScene::moveDown()
{
}

void ResolutionScene::nextResolution()
{
}

void ResolutionScene::executeSelectedOption()
{
}

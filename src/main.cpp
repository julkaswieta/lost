#include "engine.h"
#include "game.h"

using namespace std;

MenuScene menu;
Level1Scene level1;
SettingsScene settings;
ControlsScene controls;

int main()
{
	Engine::Start(1280, 720, "Lost", &controls);
}
#include "engine.h"
#include "game.h"

using namespace std;

MainMenuScene menu;
Level1Scene level1;
SettingsScene settings;
ControlsScene controls;
VolumeScene volume;

int main()
{
	Engine::Start(1280, 720, "Lost", &menu);
}
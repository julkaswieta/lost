#include "engine.h"
#include "game.h"

using namespace std;

MainMenuScene menu;
Level1Scene level1;
SettingsScene settings;
ControlsScene controls;
VolumeScene volume;
ResolutionScene resolution;

int main()
{
	Engine::Start("Lost", &menu);
}
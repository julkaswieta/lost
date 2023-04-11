#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
Level1Scene level1;
SettingsScene settings;

int main()
{
	Engine::Start(1280, 720, "Lost", &menu);
}
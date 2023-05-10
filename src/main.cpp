// this line will need to be here for the final release build in order to disable the console window in the background
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup") 
/**
* main.cpp: game starting point
*
* Author: "Lost" devs
* Last modified: 09/05/2023
*/
#include "game.h"
#include "engine.h"
#include "scenes/scene_level_menu.h"
#include "scenes/scene_level_1.h"
#include "scenes/scene_level_2.h"
#include "scenes/scene_level_3.h"

using namespace std;

MainMenuScene menu;
LevelMenuScene levelMenu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
SettingsScene settings;
ControlsScene controls;
VolumeScene volume;
ResolutionScene resolution;
EndScene endScene;
EndLevelScene endLevel;

vector<Scene*> levels = { &level1, &level2, &level3, &endScene };

int main()
{
	// Start the game
	Engine::Start("Lost", &levelMenu);
}
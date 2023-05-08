// this line will need to be here for the final release build in order to diable the console window in the background
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup") 
/**
* main.cpp: game starting point
*
* Author: "Lost" devs
* Last modified: 04/05/2023
*/
#include "engine.h"
#include "game.h"

using namespace std;

MainMenuScene menu;
Level1Scene level1;
SettingsScene settings;
ControlsScene controls;
VolumeScene volume;
ResolutionScene resolution;
EndScene endScene;

int main()
{
	Engine::Start("Lost", &endScene);
}
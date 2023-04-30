#include "save_system.h"
#include <fstream>

using namespace std;

const string SaveSystem::settingsFilePath = "settings_save.txt";
const string SaveSystem::gameSettingsFilePath = "game_save.txt";

void SaveSystem::saveSettings() {
	ofstream settingsSave;
	settingsSave.open(settingsFilePath);
	// fill the file
	settingsSave.close();
}

void SaveSystem::loadSettings() {

}

void SaveSystem::saveGame() {

}

void SaveSystem::loadGame() {

}

void SaveSystem::createAndOpenSettingsFile(){

}

void SaveSystem::createAndOpenGameFile(){

}



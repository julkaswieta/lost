#include "save_system.h"
#include <fstream>
#include "controls.h"

using namespace std;

const string SaveSystem::settingsFilePath = "settings_save.txt";
const string SaveSystem::gameSettingsFilePath = "game_save.txt";

int SaveSystem::Volume = 50;
string SaveSystem::Resolution = "1080 x 720";
int SaveSystem::WindowMode = 1;

void SaveSystem::saveSettings() {
	ofstream settingsSave;
	settingsSave.open(settingsFilePath);
	// fill the file
	settingsSave << to_string(Volume) << "\n";
	settingsSave << Resolution << "\n";
	settingsSave << WindowMode << "\n";
	settingsSave << Controls::saveMappings();
	settingsSave.close();
}

void SaveSystem::loadSettings() {

}

void SaveSystem::saveGame() {

}

void SaveSystem::loadGame() {

}

void SaveSystem::updateVolume(int newVolume)
{
	Volume = newVolume;
}

void SaveSystem::updateResolution(std::string newResolution)
{
	Resolution = newResolution;
}

void SaveSystem::updateWindowMode(int newWindowMode)
{
	WindowMode = newWindowMode;
}
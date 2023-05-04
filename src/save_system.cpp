#include "save_system.h"
#include <fstream>
#include "controls.h"
#include "scenes/scene_resolution.h"

using namespace std;
using namespace sf;

const string SaveSystem::settingsFilePath = "settings_save.txt";
const string SaveSystem::gameSettingsFilePath = "game_save.txt";

// default settings
int SaveSystem::Volume = 50;
int SaveSystem::ResolutionIndex = 2;
Vector2u SaveSystem::Resolution = { 1920, 1080 };
int SaveSystem::WindowMode = 0;

void SaveSystem::saveSettings() {
	ofstream settingsSave;
	settingsSave.open(settingsFilePath);
	settingsSave << to_string(Volume) << "\n";
	settingsSave << ResolutionIndex << "\n";
	settingsSave << WindowMode << "\n";
	settingsSave << Controls::saveMappings();
	settingsSave.close();
}

void SaveSystem::loadSettings() {
	vector<string> saveContents;
	string saveLine;
	ifstream settingsSave(settingsFilePath);
	while (getline(settingsSave, saveLine)) {
		saveContents.push_back(saveLine);
	}
	settingsSave.close();

	if (saveContents.size() == 6)
	{
		Volume = stoi(saveContents[0]);
		ResolutionIndex = stoi(saveContents[1]);
		WindowMode = stoi(saveContents[2]);

		auto start = saveContents.begin() + 3;
		auto end = saveContents.end();
		vector<string> mappings(3);
		copy(start, end, mappings.begin());
		Controls::loadMappings(mappings);
		Resolution = ResolutionScene::getResolution(ResolutionIndex);
	}
}

void SaveSystem::saveGame() {

}

void SaveSystem::loadGame() {

}

void SaveSystem::updateVolume(int newVolume) { Volume = newVolume; }

void SaveSystem::updateResolutionIndex(int newResolutionIndex) { ResolutionIndex = newResolutionIndex; }

void SaveSystem::updateResolution(Vector2u newResolution) { Resolution = newResolution; }

void SaveSystem::updateWindowMode(int newWindowMode) { WindowMode = newWindowMode; }

int SaveSystem::getVolume() { return Volume; }

int SaveSystem::getResolutionIndex() { return ResolutionIndex; }

Vector2u SaveSystem::getResolution() { return Resolution; }

int SaveSystem::getWindowMode() { return WindowMode; }

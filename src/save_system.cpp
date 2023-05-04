/**
* save_system.cpp: implementation for SaveSystem class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#include "save_system.h"
#include <fstream>
#include "controls.h"
#include "scenes/scene_resolution.h"

using namespace std;
using namespace sf;

// save files paths
const string SaveSystem::settingsFilePath = "settings_save.txt";
const string SaveSystem::gameSaveFilePath = "game_save.txt";

// default settings - get loaded if no settings save file exists 
int SaveSystem::Volume = 50;
int SaveSystem::ResolutionIndex = 2;
Vector2u SaveSystem::Resolution = { 1920, 1080 };
int SaveSystem::WindowMode = 0;
int SaveSystem::DeathCounter = 0;
int SaveSystem::LastLevelCompleted = 0;
vector<time_t> SaveSystem::LevelBestTimes = {time(NULL), (time_t)(-1)}; //test times

// Saves each settings on a separate line in a txt file
void SaveSystem::saveSettings() {
	ofstream settingsSave;
	settingsSave.open(settingsFilePath);
	settingsSave << to_string(Volume) << "\n";
	settingsSave << ResolutionIndex << "\n";
	settingsSave << WindowMode << "\n";
	settingsSave << Controls::saveMappings();
	settingsSave.close();
	cout << "Settings saved\n";
}

// Loads and processes settings from a txt file
void SaveSystem::loadSettings() {
	vector<string> saveContents;
	string saveLine;
	ifstream settingsSave(settingsFilePath);
	while (getline(settingsSave, saveLine)) {
		saveContents.push_back(saveLine);
	}
	settingsSave.close();

	// process content loaded from save file
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
	cout << "Settings loaded\n";
}

// saves the game status to a txt file
void SaveSystem::saveGame() {
	ofstream gameSave;
	gameSave.open(gameSaveFilePath);
	gameSave << LastLevelCompleted << "\n";
	gameSave << DeathCounter << "\n";
	saveLevelTimes(gameSave);
	gameSave.close();
	cout << "game saved\n";
}

// formats and outputs level times to game save file
inline void SaveSystem::saveLevelTimes(ofstream& gameSave) {
	for (int i = 0; i < LevelBestTimes.size(); ++i) {
		gameSave << to_string(i) << "," << to_string(LevelBestTimes[i]) << "\n";
	}
}

// loads and processes game status from a file
void SaveSystem::loadGame() {
	vector<string> saveContents;
	string saveLine;
	ifstream gameSave(gameSaveFilePath);
	while (getline(gameSave, saveLine)) {
		saveContents.push_back(saveLine);
	}
	gameSave.close();

	cout << "game loaded\n";
}

// getters and setters
void SaveSystem::updateVolume(int newVolume) { Volume = newVolume; }

void SaveSystem::updateResolutionIndex(int newResolutionIndex) { ResolutionIndex = newResolutionIndex; }

void SaveSystem::updateResolution(Vector2u newResolution) { Resolution = newResolution; }

void SaveSystem::updateWindowMode(int newWindowMode) { WindowMode = newWindowMode; }

int SaveSystem::getVolume() { return Volume; }

int SaveSystem::getResolutionIndex() { return ResolutionIndex; }

Vector2u SaveSystem::getResolution() { return Resolution; }

int SaveSystem::getWindowMode() { return WindowMode; }

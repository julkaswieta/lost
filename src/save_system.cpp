/**
* save_system.cpp: implementation for SaveSystem class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#include "save_system.h"
#include <fstream>
#include <filesystem>
#include <ShlObj.h>
#include "controls.h"
#include "scenes/scene_resolution.h"

using namespace std;
using namespace sf;

// save files paths
std::filesystem::path SaveSystem::settingsFilePath;
std::filesystem::path SaveSystem::gameSaveFilePath;
PWSTR path_temp;

// default settings - get loaded if no settings save file exists 
int SaveSystem::Volume = 50;
int SaveSystem::ResolutionIndex = 2;
Vector2u SaveSystem::Resolution = { 1920, 1080 };
int SaveSystem::WindowMode = 0;
int SaveSystem::DeathCounter = 0;
int SaveSystem::LastLevelCompleted = 0;
vector<float> SaveSystem::LevelBestTimes = {};
vector<string> SaveSystem::Collected = {};

void SaveSystem::initialiseSaveSystem() {
	auto folderPath = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_temp);
	if (folderPath != S_OK) {
		CoTaskMemFree(path_temp);
		cout << "Save system initialisation failed";
	}
	else {
		
		settingsFilePath = path_temp;
		gameSaveFilePath = path_temp;
		CoTaskMemFree(path_temp);
		settingsFilePath.append("Lost");
		gameSaveFilePath.append("Lost");
		if (CreateDirectory(settingsFilePath.string().c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
			settingsFilePath.append("settings_save.txt");
			gameSaveFilePath.append("game_save.txt");
			cout << settingsFilePath << "\n";
		}

	}
}

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
	cout << settingsFilePath << "\n";	
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
	saveCollected(gameSave);
	gameSave.close();
	cout << "game saved\n";
}

// formats and outputs level times to game save file
void SaveSystem::saveLevelTimes(ofstream& gameSave) {
	if (!LevelBestTimes.empty()) {
		for (int i = 0; i < LevelBestTimes.size() - 1; ++i) {
			gameSave << to_string(LevelBestTimes[i]) << ",";
		}
		gameSave << to_string(LevelBestTimes[LevelBestTimes.size() - 1]) + "\n";
	}
	else
		gameSave << "\n";
}

void SaveSystem::saveCollected(ofstream& gameSave) {
	if (!Collected.empty()) {
		for (int i = 0; i < Collected.size() - 1; ++i) {
			gameSave << Collected[i] << ",";
		}
		gameSave << Collected[Collected.size() - 1];
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

	//process save contents
	if (saveContents.size() >= 2) {
		LastLevelCompleted = stoi(saveContents[0]);
		DeathCounter = stoi(saveContents[1]);
		cout << DeathCounter << "\n";
		if(saveContents.size() >= 3)
			loadLevelTimes(saveContents[2]);
		if(saveContents.size() >= 4)
			loadCollected(saveContents[3]);
	}

	cout << "game loaded\n";
}

void SaveSystem::loadLevelTimes(string levelsSave) {
	stringstream ss(levelsSave); 
	if (!levelsSave.empty()) {
		while (ss.good()) {
			string levelTime;
			getline(ss, levelTime, ',');
			LevelBestTimes.push_back(stof(levelTime));
		}
	}
}

void SaveSystem::loadCollected(string collectedSave = NULL) {
	stringstream ss(collectedSave);
	if (!collectedSave.empty()) {
		while (ss.good()) {
			string star;
			getline(ss, star, ',');
			Collected.push_back(star);
		}
	}
}

void SaveSystem::resetData() {
	remove(gameSaveFilePath);
	DeathCounter = 0;
	LastLevelCompleted = 0;
	Collected.clear();
	LevelBestTimes.clear();
}

void SaveSystem::addCollected(vector<string> collected) { 
	for (string star : collected) {
		if (std::find(Collected.begin(), Collected.end(), star) == Collected.end())
		{
			Collected.push_back(star);
		}
	}
}

// getters and setters for settings
void SaveSystem::updateVolume(int newVolume) { Volume = newVolume; }

void SaveSystem::updateResolutionIndex(int newResolutionIndex) { ResolutionIndex = newResolutionIndex; }

void SaveSystem::updateResolution(Vector2u newResolution) { Resolution = newResolution; }

void SaveSystem::updateWindowMode(int newWindowMode) { WindowMode = newWindowMode; }

int SaveSystem::getVolume() { return Volume; }

int SaveSystem::getResolutionIndex() { return ResolutionIndex; }

Vector2u SaveSystem::getResolution() { return Resolution; }

int SaveSystem::getWindowMode() { return WindowMode; }

// getters and setters for game save
void SaveSystem::setDeathCounter(int newDeathCount) { DeathCounter = newDeathCount; }

void SaveSystem::setLastLevelCompleted(int levelNumber) { LastLevelCompleted = levelNumber; }

void SaveSystem::addNewLevelTime(int levelNumber, float newTime) { 
	if (LevelBestTimes.size() < levelNumber)
		LevelBestTimes.push_back(0.f);

	LevelBestTimes[levelNumber - 1] = newTime;
}

int SaveSystem::getDeathCount() { return DeathCounter; }

int SaveSystem::getLastLevelCompleted() { return LastLevelCompleted; }

float SaveSystem::getLevelBestTime(int levelNumber) { 
	if (LevelBestTimes.size() >= levelNumber)
		return LevelBestTimes[levelNumber - 1];
	else
		return 0.0f;
}

const vector<float> &SaveSystem::getLevelBestTimes() { return LevelBestTimes; }

vector<string> SaveSystem::getCollected() { return Collected; }

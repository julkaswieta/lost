/**
* save_system.h: header file for Controls class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#pragma once

#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include "SFML/System/Vector2.hpp"

/// A class for storing settings and game progress
/// Provides loading and saving functionality 
class SaveSystem {
public:
	static void initialiseSaveSystem();
	static void saveSettings();
	static void loadSettings();
	static void saveGame();
	static void loadGame();
	static void resetData();

	static void updateVolume(int newVolume);
	static void updateResolutionIndex(int newResolutionIndex);
	static void updateResolution(sf::Vector2u newResolution);
	static void updateWindowMode(int newWindowMode);

	static int getVolume();
	static int getResolutionIndex();
	static sf::Vector2u getResolution();
	static int getWindowMode();

	static void setDeathCounter(int newDeathCount);
	static void setLastLevelCompleted(int levelNumber);
	static void addNewLevelTime(int levelNumber, float newTime);

	static int getDeathCount();
	static int getLastLevelCompleted();
	static const std::vector<float> &getLevelBestTimes();

private:
	static std::filesystem::path settingsFilePath;
	static std::filesystem::path gameSaveFilePath;

	static int Volume;
	static int ResolutionIndex;
	static sf::Vector2u Resolution;
	static int WindowMode;

	static int LastLevelCompleted;
	static int DeathCounter;
	static std::vector<float> LevelBestTimes;

	static void saveLevelTimes(std::ofstream& gameSave);
};
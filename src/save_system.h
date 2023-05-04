#pragma once
#include <string>
#include <ctime>
#include <vector>
#include "SFML/System/Vector2.hpp"

class SaveSystem {
public:
	static void saveSettings();
	static void loadSettings();
	static void saveGame();
	static void loadGame();

	static void updateVolume(int newVolume);
	static void updateResolutionIndex(int newResolutionIndex);
	static void updateResolution(sf::Vector2u newResolution);
	static void updateWindowMode(int newWindowMode);

	static int getVolume();
	static int getResolutionIndex();
	static sf::Vector2u getResolution();
	static int getWindowMode();

private:
	static const std::string settingsFilePath;
	static const std::string gameSettingsFilePath;

	static int Volume;
	static int ResolutionIndex;
	static sf::Vector2u Resolution;
	static int WindowMode;

	static std::vector<std::time_t> LevelBestTimes;
	static int LastLevelCompleted;
	static int DeathCounter;
};
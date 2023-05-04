#pragma once
#include <string>
#include <ctime>
#include <vector>

class SaveSystem {
public:
	static void saveSettings();
	static void loadSettings();
	static void saveGame();
	static void loadGame();

	static void updateVolume(int newVolume);
	static void updateResolution(std::string newResolution);
	static void updateWindowMode(int newWindowMode);

private:
	static const std::string settingsFilePath;
	static const std::string gameSettingsFilePath;

	static int Volume;
	static std::string Resolution;
	static int WindowMode;

	static std::vector<std::time_t> LevelBestTimes;
	static int LastLevelCompleted;
	static int DeathCounter;
};
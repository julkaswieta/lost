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

private:
	static const std::string settingsFilePath;
	static const std::string gameSettingsFilePath;

	static int Volume;
	static std::string Resolution;
	static int WindowMode;
	// TODO: add mappings

	static std::vector<std::time_t> LevelBestTimes;
	static int LastLevelCompleted;
	static int DeathCounter;

	static void createAndOpenSettingsFile();
	static void createAndOpenGameFile();
	static void loadDefaultSettings();
};
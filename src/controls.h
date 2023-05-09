/**
* controls.h: header file for Controls class
* 
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#pragma once

#include "SFML/Window/Keyboard.hpp"
#include <string>
#include <vector>
#include "Gamepad.h"
#include "GamepadMgr.h"

/// A class for storingand managing control mappings
class Controls {
public:
	static sf::Keyboard::Key MoveLeft;
	static sf::Keyboard::Key MoveRight;
	static sf::Keyboard::Key Jump;
	static sf::Keyboard::Key MenuDown;
	static sf::Keyboard::Key MenuUp;
	static sf::Keyboard::Key MenuSelect;
	static sf::Keyboard::Key Exit;
	static sf::Keyboard::Key NextOption;
	static sf::Keyboard::Key PreviousOption;
	static sf::Keyboard::Key Continue;


	static std::string toString(sf::Keyboard::Key key);
	static std::string saveMappings();
	static void loadMappings(std::vector<std::string> &mappingsFromFile);
};
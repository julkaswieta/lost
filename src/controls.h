#pragma once
#include "SFML/Window/Keyboard.hpp"
#include <string>
#include "Gamepad.h"
#include "GamepadMgr.h"

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


	static std::string toString(sf::Keyboard::Key key);
};
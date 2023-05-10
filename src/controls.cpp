/**
* controls.cpp: implementation for Controls class
*
* Author: Julia Swietochowska
* Last modified: 04/05/2023
*/
#include "controls.h"

#include <SFML/System/String.hpp>

using namespace sf;
using namespace std;

// Define keyboard mappings for different controls
Keyboard::Key Controls::MoveLeft = Keyboard::Left;
Keyboard::Key Controls::MoveRight = Keyboard::Right;
Keyboard::Key Controls::Jump = Keyboard::Up;
Keyboard::Key Controls::MenuDown = Keyboard::Down;
Keyboard::Key Controls::MenuUp = Keyboard::Up;
Keyboard::Key Controls::MenuSelect = Keyboard::Enter;
Keyboard::Key Controls::Exit = Keyboard::Escape;
Keyboard::Key Controls::NextOption = Keyboard::Right;
Keyboard::Key Controls::PreviousOption = Keyboard::Left;
Keyboard::Key Controls::Continue = Keyboard::Space;

// Convert Keyboard::Key to string
string Controls::toString(Keyboard::Key key) {
	return Keyboard::getDescription(Keyboard::delocalize(key)).toAnsiString();
}

// Convert keyboard mappings to a string for saving to a file
string Controls::saveMappings()
{
	return to_string((int)Keyboard::delocalize(MoveLeft)) + "\n"
		+ to_string((int)Keyboard::delocalize(MoveRight)) + "\n"
		+ to_string((int)Keyboard::delocalize(Jump));
}

// Load keyboard mappings from a vector of strings, which were read from a file
void Controls::loadMappings(vector<string>& mappingsFromFile)
{
	// Check that there are exactly 3 mappings in the file
	if (mappingsFromFile.size() == 3) {
		// Convert the saved scancode back to Keyboard::Key and localize it
		MoveLeft = Keyboard::localize((Keyboard::Scancode)stoi(mappingsFromFile[0]));
		MoveRight = Keyboard::localize((Keyboard::Scancode)stoi(mappingsFromFile[1]));
		Jump = Keyboard::localize((Keyboard::Scancode)stoi(mappingsFromFile[2]));
	}
}

// This code defines keyboard mappings for different controls, converts Keyboard::Key to string, 
// and provides functions to save and load these mappings to/from a file. This allows the user 
// to customize their controls and save them for future use.
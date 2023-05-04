#include "controls.h"
#include "SFML/System/String.hpp"

using namespace sf;
using namespace std;

Keyboard::Key Controls::MoveLeft = Keyboard::Left;
Keyboard::Key Controls::MoveRight = Keyboard::Right;
Keyboard::Key Controls::Jump = Keyboard::Up;
Keyboard::Key Controls::MenuDown = Keyboard::Down;
Keyboard::Key Controls::MenuUp = Keyboard::Up;
Keyboard::Key Controls::MenuSelect = Keyboard::Enter;
Keyboard::Key Controls::Exit = Keyboard::Escape;
Keyboard::Key Controls::NextOption = Keyboard::Right;
Keyboard::Key Controls::PreviousOption = Keyboard::Left;

string Controls::toString(Keyboard::Key key) {
	return Keyboard::getDescription(Keyboard::delocalize(key)).toAnsiString();
}

string Controls::saveMappings()
{
	return to_string((int)Keyboard::delocalize(MoveLeft)) + "\n"
		+ to_string((int)Keyboard::delocalize(MoveRight)) + "\n"
		+ to_string((int)Keyboard::delocalize(Jump));
}

void Controls::loadMappings(vector<string> &mappingsFromFile)
{
	if (mappingsFromFile.size() == 3) {
		MoveLeft = Keyboard::localize((Keyboard::Scancode)stoi(mappingsFromFile[0]));
		MoveRight = Keyboard::localize((Keyboard::Scancode)stoi(mappingsFromFile[1]));
		Jump = Keyboard::localize((Keyboard::Scancode)stoi(mappingsFromFile[2]));
	}
}

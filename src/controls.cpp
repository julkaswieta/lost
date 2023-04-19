#include "controls.h"
#include "SFML/System/String.hpp"

using namespace sf;

Keyboard::Key Controls::MoveLeft = Keyboard::Left;
Keyboard::Key Controls::MoveRight = Keyboard::Right;
Keyboard::Key Controls::Jump = Keyboard::Up;
Keyboard::Key Controls::MenuDown = Keyboard::Down;
Keyboard::Key Controls::MenuUp = Keyboard::Up;
Keyboard::Key Controls::MenuSelect = Keyboard::Enter;
Keyboard::Key Controls::Exit = Keyboard::Escape;

std::string Controls::toString(Keyboard::Key key) {
	return Keyboard::getDescription(Keyboard::delocalize(key)).toAnsiString();
}

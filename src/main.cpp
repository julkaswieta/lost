#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 600;

CircleShape shape(100.f);

void Load() {
	shape.setFillColor(Color::Green);
}

void HandleEvents(RenderWindow& window) {
	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			window.close();
		}
	}

	//quit via Esc
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}
}

void Update() {
	static Clock clock;
	float dt = clock.restart().asSeconds();
}

void Render(RenderWindow& window) {
	window.draw(shape);
}

int main() {
	RenderWindow window(VideoMode({ GAME_WIDTH, GAME_HEIGHT}), "Space Invaders");
	Load();
	while (window.isOpen()) {
		window.clear();
		HandleEvents(window);
		Update();
		Render(window);
		window.display();
	}
	return 0;
}
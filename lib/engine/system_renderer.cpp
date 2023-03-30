#include "system_renderer.h"
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable*> sprites;
static RenderWindow* rw;

// initialises the render window
void Renderer::Initialise(sf::RenderWindow& r) { rw = &r; }

// empties the queue of sprites to be rendered
void Renderer::Shutdown() {
  while (!sprites.empty())
    sprites.pop();
}

void Renderer::Update(const double&) {}

// renders all sprites added to the renderer
void Renderer::Render() {
  if (rw == nullptr) {
    throw("No render window set! ");
  }
  while (!sprites.empty()) {
    rw->draw(*sprites.front());
    sprites.pop();
  }
}

// adds a sprite to be rendered
void Renderer::queue(const sf::Drawable* s) { sprites.push(s); }
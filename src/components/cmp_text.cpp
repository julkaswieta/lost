#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

void TextComponent::Update(double dt) {}

void TextComponent::Render() { Renderer::Queue(&text); }

TextComponent::TextComponent(Entity* const p, const std::string& str)
    : Component(p), string(str) {
  text.setString(string);
  font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
  text.setFont(*font);
}

sf::Text& TextComponent::getText() {
    return text;
}

void TextComponent::setText(const std::string& str) {
  string = str;
  text.setString(string);
}

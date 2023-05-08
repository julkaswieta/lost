#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

void TextComponent::Update(double dt) {}

void TextComponent::Render() { Renderer::Queue(&text); }

TextComponent::TextComponent(Entity* const p, const std::string& str)
    : Component(p), string(str) {
    text.setString(string);
    font = Resources::get<sf::Font>("PixeloidSansBold.ttf");
    text.setFont(*font);
}

void TextComponent::SetText(const std::string& str) {
    string = str;
    text.setString(string);
}

void TextComponent::SetColor(const sf::Color col) {
    text.setFillColor(col);
}

sf::Text& TextComponent::getText() {
    return text;
}

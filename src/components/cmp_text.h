#pragma once

#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

class TextComponent : public Component {
public:
	TextComponent() = delete;
	explicit TextComponent(Entity* p, const std::string& str = "");
	void Update(double dt) override;

	void Render() override;

	~TextComponent() override = default;

	void SetText(const std::string& str);
	void SetColor(const sf::Color col);

protected:
	std::shared_ptr<sf::Font> font;
	std::string string;
	sf::Text text;
};

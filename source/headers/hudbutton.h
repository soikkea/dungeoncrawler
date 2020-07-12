#pragma once

#include <string>

#include <SFML/Graphics.hpp>

class HudButton : public sf::Drawable, public ::sf::Transformable {
public:
	HudButton(const sf::Vector2f& position, const std::string& name);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2f position;
	sf::Sprite sprite;
	bool active = false;
	std::string name;
};

class IncrementButton : public HudButton {
public:
	IncrementButton(const sf::Vector2f& position, const std::string& name);
	static const sf::Vector2f SIZE;
};

class MenuButton : public HudButton {
public:
	MenuButton(const sf::Vector2f& position, const std::string& name, const std::string& text);
	static const sf::Vector2f SIZE;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
protected:
	std::string text;
	sf::Text textObject;
};
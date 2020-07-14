#include "hudbutton.h"
#include "assets.h"

HudButton::HudButton(const sf::Vector2f& position, const std::string& name) :
	position(position),
	name(name)
{
	setPosition(position);
}

void HudButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!active)
		return;
	target.draw(sprite);
}

const sf::Vector2f IncrementButton::SIZE = sf::Vector2f(12.f, 12.f);

IncrementButton::IncrementButton(const sf::Vector2f& position, const std::string& name) :
	HudButton(position, name)
{
	sprite = sf::Sprite(Assets::get().getTexture("increment_button"));
	sprite.setPosition(position);
}

const sf::Vector2f MenuButton::SIZE = sf::Vector2f(300.f, 100.f);

MenuButton::MenuButton(const sf::Vector2f& position, const std::string& name, const std::string& text) :
	HudButton(position, name),
	text(text)
{
	sprite = sf::Sprite(Assets::get().getTexture("menu_button"));
	sprite.setPosition(position);
	active = true;
	textObject = sf::Text(text, Assets::get().getFont("main"), 16);
	auto textBounds = textObject.getLocalBounds();
	auto textSize = sf::Vector2f(textBounds.width, textBounds.height);
	auto textOffset = (SIZE - textSize) * 0.5f;

	textObject.setPosition(
		position +
		textOffset
	);
}

void MenuButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!active)
		return;
	HudButton::draw(target, states);
	target.draw(textObject);
}
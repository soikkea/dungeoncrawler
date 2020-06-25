#include "hudbutton.h"
#include "assets.h"

HudButton::HudButton(const sf::Vector2f& position,
	const sf::Rect<float>& boundingBox, const std::string& name) :
	position(position),
	boundingBox(boundingBox),
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
	HudButton(position, sf::Rect<float>(position, SIZE), name)
{
	sprite = sf::Sprite(Assets::get().getTexture("increment_button"));
	sprite.setPosition(position);
}

#include "assets.h"
#include "hudbutton.h"

void Assets::LoadTextures()
{
	// IncrementButton texture
	sf::RenderTexture texture;
	texture.create((unsigned)IncrementButton::SIZE.x, (unsigned)IncrementButton::SIZE.y);
	texture.clear();
	sf::RectangleShape background(IncrementButton::SIZE);
	background.setFillColor(sf::Color::Yellow);

	texture.draw(background);

	const float barThickness = 2.f;
	sf::RectangleShape vertical(sf::Vector2f(barThickness, 0.8f * IncrementButton::SIZE.y));
	vertical.setFillColor(sf::Color::Black);
	vertical.setPosition(0.5f * (IncrementButton::SIZE.x - barThickness), 0.1f * IncrementButton::SIZE.y);
	texture.draw(vertical);

	sf::RectangleShape horizontal(sf::Vector2f(0.8f * IncrementButton::SIZE.x, barThickness));
	horizontal.setFillColor(sf::Color::Black);
	horizontal.setPosition(0.1f * IncrementButton::SIZE.x, 0.5f * (IncrementButton::SIZE.y - barThickness));
	texture.draw(horizontal);

	texture.display();

	_textures["increment_button"] = texture.getTexture();
}

void Assets::LoadFonts()
{
}

Assets::Assets()
{
}

Assets::~Assets()
{
}
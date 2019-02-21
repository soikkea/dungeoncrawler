#include "sprite.h"

Sprite::Sprite() :
	m_vertices(sf::Quads, 4)
{
	m_vertices[0].position = sf::Vector2f(0.f, 0.f);
	m_vertices[1].position = sf::Vector2f(50.f, 0.f);
	m_vertices[2].position = sf::Vector2f(50.f, 50.f);
	m_vertices[3].position = sf::Vector2f(0.f, 50.f);

	m_vertices[0].color = sf::Color::Magenta;
	m_vertices[1].color = sf::Color::Magenta;
	m_vertices[2].color = sf::Color::Magenta;
	m_vertices[3].color = sf::Color::Magenta;
}
Sprite::~Sprite()
{
}
void Sprite::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// Apply the entity's transform -- combine it with the one that was passed by the caller
	states.transform *= getTransform();

	// Draw the vertex array
	target.draw(m_vertices, states);
}


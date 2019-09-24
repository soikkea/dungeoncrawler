#include "globals.h"
#include "sprite.h"
#include "level.h"

Sprite::Sprite() :
	m_vertices(sf::Quads, 4),
	_tileX(0),
	_tileY(0)
{
	const float grid_size = (float)globals::TILE_SIZE;
	m_vertices[0].position = sf::Vector2f(0.f, 0.f);
	m_vertices[1].position = sf::Vector2f(grid_size, 0.f);
	m_vertices[2].position = sf::Vector2f(grid_size, grid_size);
	m_vertices[3].position = sf::Vector2f(0.f, grid_size);

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

/*
 * Return the center point of the sprite in world coordinates
 */
const sf::Vector2f Sprite::getWorldCenter() const
{
	sf::Vector2f position = getPosition();
	const float centerOffset = ((float)globals::TILE_SIZE * 0.5f);
	return position + sf::Vector2f(centerOffset, centerOffset);
}

bool Sprite::moveStep(Direction direction, const Level & level)
{
	sf::Vector2i directionVector = getDirectionUnitVector(direction);
	int newX = _tileX + directionVector.x;
	int newY = _tileY + directionVector.y;
	if (!level.isEmpty(newX, newY)) return false;
	setTilePos(newX, newY);
	return true;
}

void Sprite::setTilePos(int x, int y)
{
	this->_tileX = x;
	this->_tileY = y;
	this->setPosition(sf::Vector2f((float)x, (float)y) * (float)globals::TILE_SIZE);
}

const sf::Vector2i Sprite::getTilePos() const
{
	return sf::Vector2i(_tileX, _tileY);
}

void Sprite::setColor(const sf::Color color)
{
	m_vertices[0].color = color;
	m_vertices[1].color = color;
	m_vertices[2].color = color;
	m_vertices[3].color = color;
}


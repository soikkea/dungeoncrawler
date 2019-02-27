#include "globals.h"
#include "sprite.h"

Sprite::Sprite() :
	m_vertices(sf::Quads, 4),
	m_worldX(0),
	m_worldY(0)
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

void Sprite::worldMove(Direction direction, int steps)
{
	switch (direction)
	{
	case NORTH:
		this->m_worldY -= steps;
		break;
	case EAST:
		this->m_worldX += steps;
		break;
	case SOUTH:
		this->m_worldY += steps;
		break;
	case WEST:
		this->m_worldX -= steps;
		break;
	default:
		break;
	}
	this->setPosition(sf::Vector2f(m_worldX, m_worldY) * (float)globals::TILE_SIZE);
}

void Sprite::moveStep(Direction direction, const Level & level)
{
	sf::Vector2i directionVector = getDirectionUnitVector(direction);
	int newX = m_worldX + directionVector.x;
	int newY = m_worldY + directionVector.y;
	if (!level.isEmpty(newX, newY)) return;
	setWorldPos(newX, newY);
}

void Sprite::setWorldPos(int x, int y)
{
	this->m_worldX = x;
	this->m_worldY = y;
	this->setPosition(sf::Vector2f(x, y) * (float)globals::TILE_SIZE);
}


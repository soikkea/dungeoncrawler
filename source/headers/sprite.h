#pragma once

#include <SFML/Graphics.hpp>

#include "globals.h"

class Level;

class Sprite : public sf::Drawable, public sf::Transformable {
public:
	Sprite();
	virtual ~Sprite();
	void moveStep(Direction direction, int steps);
	void moveStep(Direction direction, const Level& level);
	void setTilePos(int x, int y);
	const sf::Vector2i getTilePos() const;
	void setColor(const sf::Color color);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	const sf::Vector2f getWorldCenter() const;
protected:
	sf::VertexArray m_vertices;
	int _tileX;
	int _tileY;
};
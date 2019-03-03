#pragma once

#include <SFML/Graphics.hpp>

#include "globals.h"

class Level;

class Sprite : public sf::Drawable, public sf::Transformable {
public:
	Sprite();
	virtual ~Sprite();
	void worldMove(Direction direction, int steps);
	void moveStep(Direction direction, const Level& level);
	void setWorldPos(int x, int y);
	void setColor(const sf::Color color);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::VertexArray m_vertices;
	int m_worldX;
	int m_worldY;
};
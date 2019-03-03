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
	const sf::Vector2i getWorldPos() const;
	void setColor(const sf::Color color);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
protected:
	sf::VertexArray m_vertices;
	int m_worldX;
	int m_worldY;
};
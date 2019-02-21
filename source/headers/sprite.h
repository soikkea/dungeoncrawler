#pragma once

#include <SFML/Graphics.hpp>

class Sprite : public sf::Drawable, public sf::Transformable {
public:
	Sprite();
	virtual ~Sprite();
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::VertexArray m_vertices;
};
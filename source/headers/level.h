#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class Level : public sf::Drawable, public sf::Transformable {
public:
	static const int EMPTY = 0;
	static const int WALL = 1;
	static const int PLAYER = 2;
	bool load(sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
	void tmpInit();
	const sf::Vector2u getPlayerStartingPos() const;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray _vertices;
	sf::Vector2u _playerStartingPos;
};
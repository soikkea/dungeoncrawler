#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class Creature;

class Level : public sf::Drawable, public sf::Transformable {
public:
	enum class TileType 
	{
		EMPTY = 0,
		FLOOR,
		WALL,
		PLAYER,
	};
	bool load(sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
	void tmpInit();
	const sf::Vector2u getPlayerStartingPos() const;
	bool isEmpty(unsigned int x, unsigned int y) const;
	int getTile(unsigned int x, unsigned int y) const;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray _vertices;
	unsigned int _width;
	unsigned int _height;
	std::vector<int> _tiles;
	sf::Vector2u _playerStartingPos;
	std::vector<std::shared_ptr<Creature>> _creatures;
	std::vector<sf::Rect<unsigned int>> _rooms;
};
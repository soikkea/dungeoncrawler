#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class Creature;
template <typename T> struct Map2D;

class Level : public sf::Drawable, public sf::Transformable {
public:
	enum class TileType 
	{
		EMPTY = 0,
		FLOOR,
		WALL,
		PLAYER,
	};
	bool load(const sf::Vector2u tileSize, const Map2D<unsigned int> & map);
	void tmpInit();
	const sf::Vector2u getPlayerStartingPos() const;
	bool isEmpty(unsigned int x, unsigned int y) const;
	int getTile(unsigned int x, unsigned int y) const;
	void populate();
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
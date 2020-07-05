#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

enum class TileVisibility
{
	UNSEEN,
	REVEALED,
	VISIBLE
};

enum class TileType
{
	EMPTY = 0,
	FLOOR,
	WALL,
	PLAYER,
	STAIRS
};

struct Tile
{
	Tile();
	bool IsType(TileType type) const;
	TileType type;
	TileVisibility visibility;
};

struct Map2D
{
	Map2D(int width, int height);
	~Map2D();
	void setTileAt(int x, int y, const Tile& tile);
	void setTileTypeAt(int x, int y, TileType tileType);
	const Tile getTileAt(int x, int y) const;
	const Tile getTileAt(sf::Vector2i point) const;
	int width;
	int height;
	std::vector<std::vector<Tile>> map;
	std::vector<sf::Rect<int>> rooms;
	static sf::Vector2i getRandomPointInsideRoom(sf::Rect<int>& room);
};

class BSPDungeon
{
public:
	static const int CORRIDOR_WIDTH = 1;
	BSPDungeon(sf::Rect<int> limits, std::shared_ptr<Map2D> map);
	static std::shared_ptr<BSPDungeon> generateDungeon(int width, int height, int depth);
	void split();
	void connect();
	void generateRoom();
	void createCorridor(int x1, int y1, int x2, int y2, int width);
	std::shared_ptr<Map2D> getMap();
protected:
	bool connectNodes(const sf::Rect<int> & leftLimits, const sf::Rect<int> & rightLimits);
	std::shared_ptr<BSPDungeon> leftChild;
	std::shared_ptr<BSPDungeon> rightChild;
	std::shared_ptr<Map2D> map;
	sf::Rect<int> limits;
	sf::Rect<int> room;
};

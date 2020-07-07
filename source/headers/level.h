#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "bspdungeon.h"
#include "item.h"

class Creature;
class Player;
struct Map2D;
enum class TileType;
struct Tile;

class Level : public sf::Drawable, public sf::Transformable {
public:
	bool load(const sf::Vector2u tileSize, const Map2D & map);
	void initializeNewDungeon();
	const sf::Vector2i getPlayerStartingPos() const;
	bool tileIsEmpty(int x, int y) const;
	bool tileBlocksVision(int x, int y) const;
	const Tile& getTile(int x, int y) const;
	void setTile(const sf::Vector2i pos, const TileType type);
	bool getLineOfSight(const sf::Vector2i& start, const sf::Vector2i& end);
	void populate();
	void update(Player& player);
	const std::vector<std::unique_ptr<Creature>>& getCreatures();
	int tileHasCreature(int x, int y) const;
	int tileHasItem(int x, int y) const;
	bool pickUpItemAt(int x, int y, Creature& user);

	bool endReached = false;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void updateFog(const sf::Vector2i& playerPos);

	sf::VertexArray _vertices;
	sf::VertexArray _fog;
	sf::Vector2u _tileSize;
	int _width;
	int _height;
	std::vector<std::vector<Tile>> _tiles;
	sf::Vector2i _playerStartingPos;
	sf::Vector2i _levelEndPos;
	std::vector<std::unique_ptr<Creature>> _creatures;
	std::vector<std::unique_ptr<Creature>> _deadCreatures;
	std::vector<std::unique_ptr<Item>> _items;
	std::vector<sf::Rect<int>> _rooms;
};
#include "level.h"
#include "globals.h"
#include "creature.h"
#include "bspdungeon.h"
#include "rng.h"
#include "player.h"


bool Level::load(const sf::Vector2u tileSize, const Map2D & map)
{
	int width = map.width;
	int height = map.height;

	// Resize the vertex array to fit the level size
	_vertices.setPrimitiveType(sf::Quads);
	_vertices.resize(width * height * 4);

	_width = width;
	_height = height;

	_tiles = std::vector<std::vector<Tile>>(map.map);

	// Populate the vertex array, with one quad per tile
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			// Get the current tile number
			auto tile = map.getTileAt(i, j);

			_tiles[i][j] = tile;

			// Get color for tile
			sf::Color tileColor;
			switch (tile.type)
			{
			case TileType::EMPTY:
				tileColor = sf::Color::Black;
				break;
			case TileType::FLOOR:
				tileColor = sf::Color(224, 224, 224);
				break;
			case TileType::WALL:
				tileColor = sf::Color::White;
				break;
			case TileType::PLAYER:
				tileColor = sf::Color::Cyan;
				_playerStartingPos = sf::Vector2u(i, j);
				break;
			default:
				tileColor = sf::Color::Cyan;
				break;
			}

			// Get a pointer to the current tile's quad
			sf::Vertex* quad = &_vertices[(i + j * width) * 4];

			// Define its 4 corners
			quad[0].position = sf::Vector2f((float) (i * tileSize.x),       (float)(j * tileSize.y));
			quad[1].position = sf::Vector2f((float) ((i + 1) * tileSize.x), (float)(j * tileSize.y));
			quad[2].position = sf::Vector2f((float) ((i + 1) * tileSize.x), (float)((j + 1) * tileSize.y));
			quad[3].position = sf::Vector2f((float) (i * tileSize.x),       (float)((j + 1) * tileSize.y));

			// Set the color of the tile
			quad[0].color = tileColor;
			quad[1].color = tileColor;
			quad[2].color = tileColor;
			quad[3].color = tileColor;
		}

	}

	_rooms = map.rooms;

	return true;
}

void Level::tmpInit()
{
	// Placeholder function for initializing the level

	const int WIDTH = 60;
	const int HEIGHT = 60;
	const int DEPTH = 4;

	auto dungeon = BSPDungeon::generateDungeon(WIDTH, HEIGHT, DEPTH);

	auto map = dungeon->getMap();

	load(sf::Vector2u(globals::TILE_SIZE, globals::TILE_SIZE), *map);

	populate();
}

const sf::Vector2u Level::getPlayerStartingPos() const
{
	return _playerStartingPos;
}

bool Level::isEmpty(int x, int y) const
{
	if (x >= _width || y >= _height) return false;
	auto tile = getTile(x, y);
	if (tile.type == TileType::WALL) return false;
	for (auto& creature : _creatures)
	{
		sf::Vector2i creaturePos = creature->getTilePos();
		if (creaturePos.x == x && creaturePos.y == y) return false;
	}
	return true;
}

const Tile& Level::getTile(int x, int y) const
{
	return _tiles.at(x).at(y);
}

void Level::setTile(const sf::Vector2i pos, const TileType type)
{
	_tiles[pos.x][pos.y].type = type;
}

bool Level::getLineOfSight(const sf::Vector2i & start, const sf::Vector2i & end)
{
	float deltaX = (float)abs(start.x - end.x);
	float deltaY = (float)abs(start.y - end.y);
	float width = deltaX > deltaY ? deltaX : deltaY;
	float height = deltaX < deltaY ? deltaX : deltaY;
	float error = width * 0.5f;
	bool loopOverX = deltaX > deltaY;
	int  s, t, sStep, tStep, sEnd;
	if (loopOverX) {
		sStep = start.x < end.x ? 1 : -1;
		tStep = start.y < end.y ? 1 : -1;
		s = start.x + sStep;
		sEnd = end.x;
		t = start.y;
	}
	else {
		sStep = start.y < end.y ? 1 : -1;
		tStep = start.x < end.x ? 1 : -1;
		s = start.y + sStep;
		sEnd = end.y;
		t = start.x;
	}
	while (s != sEnd) {
		if (loopOverX) {
			// s, t
			if (!isEmpty(s, t)) return false;
		}
		else {
			// t, s
			if (!isEmpty(t, s)) return false;
		}
		error = error + height;
		if (error >= width) {
			t += tStep;
			error = error - width;
		}
		s += sStep;
	}
	return true;
}

// Populate the level with NPCs
void Level::populate()
{
	for (size_t i = 1; i < _rooms.size(); i++)
	{
		auto & room = _rooms.at(i);
		auto x = rng::randomIntBetween(room.left + 2, room.left + room.width - 3);
		auto y = rng::randomIntBetween(room.top + 2, room.top + room.height - 3);
		_creatures.push_back(std::make_unique<Creature>(x, y));
	}
}

void Level::update(Player& player)
{
	// Make sure creatures do not walk where the player is
	setTile(player.playerCreature.getTilePos(), TileType::WALL);
	auto it = _creatures.begin();
	while (it != _creatures.end()) {
		(*it)->update(*this, player);
		if (!(*it)->isAlive()) {
			_deadCreatures.push_back(std::move(*it));
			it = _creatures.erase(it);
		}
		it++;
	}
	setTile(player.playerCreature.getTilePos(), TileType::FLOOR);
}

const std::vector<std::unique_ptr<Creature>>& Level::getCreatures()
{
	return _creatures;
}

void Level::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// Apply the transform
	states.transform *= getTransform();

	// Draw the vertex array
	target.draw(_vertices, states);

	for (size_t i = 0; i < _creatures.size(); i++)
	{
		_creatures.at(i)->draw(target, states);
	}
}

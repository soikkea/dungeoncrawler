#include "level.h"
#include "globals.h"
#include "creature.h"
#include "bspdungeon.h"
#include "rng.h"
#include "player.h"


bool Level::load(const sf::Vector2u tileSize, const Map2D<unsigned int> & map)
{
	unsigned int width = map.width;
	unsigned int height = map.height;

	// Resize the vertex array to fit the level size
	_vertices.setPrimitiveType(sf::Quads);
	_vertices.resize(width * height * 4);

	_width = width;
	_height = height;

	_tiles.resize(width * height);

	// Populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			// Get the current tile number
			int tileNumber = map.map[i + j * width];

			_tiles[i + j * width] = tileNumber;

			// Get color for tile
			sf::Color tileColor;
			switch (tileNumber)
			{
			case static_cast<int>(Level::TileType::EMPTY):
				tileColor = sf::Color::Black;
				break;
			case static_cast<int>(Level::TileType::FLOOR):
				tileColor = sf::Color::Color(224, 224, 224);
				break;
			case static_cast<int>(Level::TileType::WALL):
				tileColor = sf::Color::White;
				break;
			case static_cast<int>(Level::TileType::PLAYER):
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

bool Level::isEmpty(unsigned int x, unsigned int y) const
{
	if (x >= _width || y >= _height) return false;
	int tile = getTile(x, y);
	if (tile == static_cast<int>(Level::TileType::WALL)) return false;
	for (auto& creature : _creatures)
	{
		sf::Vector2i creaturePos = creature->getTilePos();
		if (creaturePos.x == x && creaturePos.y == y) return false;
	}
	return true;
}

int Level::getTile(unsigned int x, unsigned int y) const
{
	return _tiles.at(x + y * _width);
}

void Level::setTile(const sf::Vector2i pos, const TileType type)
{
	int index = pos.x + _width * pos.y;
	_tiles[index] = static_cast<int>(type);
}

// Populate the level with NPCs
void Level::populate()
{
	for (int i = 1; i < _rooms.size(); i++)
	{
		auto & room = _rooms.at(i);
		auto x = random::randomIntBetween(room.left + 2, room.left + room.width - 3);
		auto y = random::randomIntBetween(room.top + 2, room.top + room.height - 3);
		_creatures.push_back(std::make_unique<Creature>(x, y));
	}
}

void Level::update(Player& player)
{
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

	for (unsigned int i = 0; i < _creatures.size(); i++)
	{
		_creatures.at(i)->draw(target, states);
	}
}

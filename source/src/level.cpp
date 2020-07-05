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

	_fog.setPrimitiveType(sf::Quads);
	_fog.resize(width * height * 4);

	_tileSize = tileSize;

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
				_playerStartingPos = sf::Vector2i(i, j);
				break;
			case TileType::STAIRS:
				tileColor = sf::Color::Magenta;
				_levelEndPos = sf::Vector2i(i, j);
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

	updateFog(_playerStartingPos);
}

const sf::Vector2i Level::getPlayerStartingPos() const
{
	return _playerStartingPos;
}

bool Level::tileIsEmpty(int x, int y) const
{
	if (tileBlocksVision(x, y)) return false;
	if (tileHasItem(x, y) >= 0) return false;
	return true;
}

bool Level::tileBlocksVision(int x, int y) const
{
	if (x >= _width || y >= _height) return true;
	auto tile = getTile(x, y);
	if (tile.type == TileType::WALL) return true;
	if (tileHasCreature(x, y) >= 0) return true;
	return false;
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
			if (tileBlocksVision(s, t)) return false;
		}
		else {
			// t, s
			if (tileBlocksVision(t, s)) return false;
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
	auto isNotSpecialTile = [&](sf::Vector2i& pos) {
		return _playerStartingPos != pos && _levelEndPos != pos;
	};
	for (size_t i = 1; i < _rooms.size(); i++)
	{
		auto & room = _rooms.at(i);
		sf::Vector2i creaturePosition = Map2D::getRandomPointInsideRoom(room);
		if (isNotSpecialTile(creaturePosition)) {
			_creatures.push_back(std::make_unique<Creature>(creaturePosition.x, creaturePosition.y));
		}

		// Create item
		if (rng::randomInt(1) == 1) {
			sf::Vector2i itemPosition = Map2D::getRandomPointInsideRoom(room);
			if (isNotSpecialTile(itemPosition) && itemPosition != creaturePosition) {
				_items.push_back(std::make_unique<HealthPotion>());
				_items.back()->setTilePos(itemPosition.x, itemPosition.y);
			}
		}
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
			if (it == _creatures.end()) break;
		}
		it++;
	}
	setTile(player.playerCreature.getTilePos(), TileType::FLOOR);

	updateFog(player.playerCreature.getTilePos());
}

const std::vector<std::unique_ptr<Creature>>& Level::getCreatures()
{
	return _creatures;
}

int Level::tileHasCreature(int x, int y) const
{
	if (_creatures.size() == 0)
		return -1;

	for (size_t i = 0; i < _creatures.size(); i++)
	{
		auto creaturePos = _creatures.at(i)->getTilePos();
		if (creaturePos.x == x && creaturePos.y == y)
			return i;
	}
	return -1;
}

int Level::tileHasItem(int x, int y) const
{
	if (_items.size() == 0)
		return -1;

	for (size_t i = 0; i < _items.size(); i++)
	{
		auto itemPos = _items.at(i)->getTilePos();
		if (itemPos.x == x && itemPos.y == y)
			return i;
	}
	return -1;
}

bool Level::pickUpItemAt(int x, int y, Creature& user)
{
	auto itemIndex = tileHasItem(x, y);
	if (itemIndex < 0)
		return false;

	auto& item = _items.at(itemIndex);

	if (user.addItem(item)) {
		_items.erase(_items.begin() + itemIndex);
		return true;
	}

	return false;
}

void Level::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// Apply the transform
	states.transform *= getTransform();

	// Draw the vertex array
	target.draw(_vertices, states);

	for (size_t i = 0; i < _items.size(); i++)
	{
		auto& item = _items.at(i);
		auto itemPos = item->getTilePos();
		if (_tiles[itemPos.x][itemPos.y].visibility == TileVisibility::VISIBLE)
			item->draw(target, states);
	}

	for (size_t i = 0; i < _creatures.size(); i++)
	{
		auto& creature = _creatures.at(i);
		auto creaturePos = creature->getTilePos();
		if (_tiles[creaturePos.x][creaturePos.y].visibility == TileVisibility::VISIBLE)
			_creatures.at(i)->draw(target, states);
	}

	target.draw(_fog, states);
}

void Level::updateFog(const sf::Vector2i& playerPos)
{
	auto visibleColor = sf::Color(0, 0, 0, 0);
	auto unseenColor = sf::Color(0, 0, 0, 255);
	auto revealedColor = sf::Color(0, 0, 0, 100);

	for (size_t x = 0; x < _width; x++)
	{
		for (size_t y = 0; y < _height; y++)
		{
			auto pos = sf::Vector2i(x, y);
			bool canBeSeen = getLineOfSight(playerPos, pos);

			sf::Vertex* quad = &_fog[(x + y * _width) * 4];

			quad[0].position = sf::Vector2f((float)(x * _tileSize.x), (float)(y * _tileSize.y));
			quad[1].position = sf::Vector2f((float)((x + 1) * _tileSize.x), (float)(y * _tileSize.y));
			quad[2].position = sf::Vector2f((float)((x + 1) * _tileSize.x), (float)((y + 1) * _tileSize.y));
			quad[3].position = sf::Vector2f((float)(x * _tileSize.x), (float)((y + 1) * _tileSize.y));

			if (canBeSeen) {

				quad[0].color = visibleColor;
				quad[1].color = visibleColor;
				quad[2].color = visibleColor;
				quad[3].color = visibleColor;

				_tiles[x][y].visibility = TileVisibility::VISIBLE;
			}
			else
			{
				switch (_tiles[x][y].visibility)
				{
				case TileVisibility::UNSEEN:
					quad[0].color = unseenColor;
					quad[1].color = unseenColor;
					quad[2].color = unseenColor;
					quad[3].color = unseenColor;
					break;
				case TileVisibility::VISIBLE:
					_tiles[x][y].visibility = TileVisibility::REVEALED;
				case TileVisibility::REVEALED:
					quad[0].color = revealedColor;
					quad[1].color = revealedColor;
					quad[2].color = revealedColor;
					quad[3].color = revealedColor;
					break;
				default:
					break;
				}
			}
		}
	}
}
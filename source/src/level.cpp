#include "level.h"
#include "globals.h"


bool Level::load(sf::Vector2u tileSize, const int * tiles, unsigned int width, unsigned int height)
{
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
			int tileNumber = tiles[i + j * width];

			_tiles[i + j * width] = tileNumber;

			// Get color for tile
			sf::Color tileColor;
			switch (tileNumber)
			{
			case Level::EMPTY:
				tileColor = sf::Color::Black;
				break;
			case Level::WALL:
				tileColor = sf::Color::White;
				break;
			case Level::PLAYER:
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
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// Set the color of the tile
			quad[0].color = tileColor;
			quad[1].color = tileColor;
			quad[2].color = tileColor;
			quad[3].color = tileColor;
		}

	}
	return true;
}

void Level::tmpInit()
{
	const int level[] =
	{
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 2, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
	};

	load(sf::Vector2u(globals::TILE_SIZE, globals::TILE_SIZE), level, 8, 5);
}

const sf::Vector2u Level::getPlayerStartingPos() const
{
	return _playerStartingPos;
}

bool Level::isEmpty(unsigned int x, unsigned int y) const
{
	if (x >= _width || y >= _height) return false;
	int tile = getTile(x, y);
	if (tile == Level::WALL) return false;
	return true;
}

int Level::getTile(unsigned int x, unsigned int y) const
{
	return _tiles.at(x + y * _width);
}

void Level::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// Apply the transform
	states.transform *= getTransform();

	// Draw the vertex array
	target.draw(_vertices, states);
}

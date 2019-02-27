#pragma once

#include <SFML/System.hpp>

namespace globals {
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;

	const int TILE_SIZE = 32;
}

enum Direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

inline sf::Vector2i getDirectionUnitVector(Direction direction) {
	switch (direction)
	{
	case NORTH:
		return sf::Vector2i(0, -1);
		break;
	case EAST:
		return sf::Vector2i(1, 0);
		break;
	case SOUTH:
		return sf::Vector2i(0, 1);
		break;
	case WEST:
		return sf::Vector2i(-1, 0);
		break;
	default:
		return sf::Vector2i(0, 0);
		break;
	}
}
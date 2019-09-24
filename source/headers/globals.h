#pragma once

#include <algorithm> // max, min
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

inline bool getOverlap(const int a1, const int a2, const int b1, const int b2, int& c, int& d)
{
	// a1 < a2 and b1 < b2 required
	if (a1 > b2 || a2 < b1) return false;
	c = std::max(a1, b1);
	d = std::min(a2, b2);
	return true;
}

inline int minIntDistance(const sf::Vector2i& a, const sf::Vector2i& b) {
	return std::min(std::abs(a.x - b.x), std::abs(a.y - b.y));
}

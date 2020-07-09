#include <vector>

#include <catch2/catch.hpp>

#include "globals.h"
#include "bspdungeon.h"
#include "level.h"

void MapFromInts(std::vector<std::vector<int>>& intMap, Map2D& map) {
	for (size_t y = 0; y < intMap.size(); y++)
	{
		auto& row = intMap[y];
		for (size_t x = 0; x < row.size(); x++)
		{
			map.setTileTypeAt(x, y, (TileType)row[x]);
		}
	}
}

TEST_CASE("Line of sight works correctly", "[level]") {
	auto map = Map2D(10, 15);

	std::vector<std::vector<int>> intMap{
		{1, 1, 1, 1, 1, 1},
		{1, 1, 2, 2, 1, 1},
		{1, 1, 2, 2, 1, 1},
		{2, 1, 2, 2, 1, 1},
		{1, 1, 1, 1, 1, 2},
		{1, 1, 1, 1, 1, 2},
		{1, 1, 1, 1, 1, 2},
		{2, 2, 2, 2, 2, 2},
		{2, 2, 2, 2, 2, 2},
		{1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1},
	};

	MapFromInts(intMap, map);

	auto level = Level();
	level.load(sf::Vector2u(globals::TILE_SIZE, globals::TILE_SIZE), map);

	auto startPos = sf::Vector2i(0, 0);

	REQUIRE(level.getTile(0, 0).IsType(TileType::FLOOR));
	REQUIRE(level.getTile(2, 1).IsType(TileType::WALL));
	REQUIRE(level.getTile(2, 2).IsType(TileType::WALL));

	SECTION( "line of sight works on floor" ) {
		REQUIRE(level.getLineOfSight(startPos, sf::Vector2i(3, 0)));
		REQUIRE(level.getLineOfSight(startPos, sf::Vector2i(1, 1)));
		REQUIRE(level.getLineOfSight(startPos, sf::Vector2i(1, 4)));
		REQUIRE(level.getLineOfSight(startPos, sf::Vector2i(0, 2)));
	}

	SECTION( "walls block line of sight" ) {
		REQUIRE(level.getLineOfSight(startPos, sf::Vector2i(0, 3)));
		REQUIRE(!level.getLineOfSight(startPos, sf::Vector2i(0, 4)));
		REQUIRE(!level.getLineOfSight(startPos, sf::Vector2i(4, 4)));
		REQUIRE(!level.getLineOfSight(startPos, sf::Vector2i(4, 1)));
	}

	SECTION( "line of sight works on diagonal" ) {
		REQUIRE(level.getLineOfSight(startPos, sf::Vector2i(1, 1)));
		REQUIRE(level.getLineOfSight(startPos, sf::Vector2i(2, 2)));
		REQUIRE(!level.getLineOfSight(startPos, sf::Vector2i(3, 3)));
		REQUIRE(!level.getLineOfSight(startPos, sf::Vector2i(4, 4)));
	}

	SECTION( "can't see through double wall" ) {
		REQUIRE(level.getLineOfSight(startPos, sf::Vector2i(3, 0)));
		REQUIRE(!level.getLineOfSight(startPos, sf::Vector2i(3, 2)));
		REQUIRE(!level.getLineOfSight(startPos, sf::Vector2i(3, 3)));
		REQUIRE(!level.getLineOfSight(startPos, sf::Vector2i(3, 4)));
		REQUIRE(!level.getLineOfSight(sf::Vector2i(0, 4), sf::Vector2i(5, 8)));
	}
}
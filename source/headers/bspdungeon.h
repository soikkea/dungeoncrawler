#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class BSPDungeon
{
public:
	BSPDungeon(sf::Rect<unsigned int> limits, std::shared_ptr<std::vector<int>> map);
	static BSPDungeon generateDungeon(int width, int height, int depth);
	void split();
	void connect();
	void generateRoom();
protected:
	std::shared_ptr<BSPDungeon> leftChild;
	std::shared_ptr<BSPDungeon> rightChild;
	std::shared_ptr<std::vector<int>> map;
	sf::Rect<int> room;
};
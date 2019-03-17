#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

template <typename T> struct Map2D
{
	Map2D(int width, int height, T initValue);
	void setValueAt(int x, int y, T value);
	T getValueAt(int x, int y);
	int width;
	int height;
	std::vector<T> map;
	std::vector<sf::Rect<T>> rooms;
};

class BSPDungeon
{
public:
	BSPDungeon(sf::Rect<unsigned int> limits, std::shared_ptr<Map2D<unsigned int>> map);
	static std::shared_ptr<BSPDungeon> generateDungeon(int width, int height, int depth);
	void split();
	void connect();
	void generateRoom();
	void createCorridor(int x1, int y1, int x2, int y2, int width);
	std::shared_ptr<Map2D<unsigned int>> getMap();
protected:
	bool connectNodes(const sf::Rect<unsigned int> & leftLimits, const sf::Rect<unsigned int> & rightLimits);
	std::shared_ptr<BSPDungeon> leftChild;
	std::shared_ptr<BSPDungeon> rightChild;
	std::shared_ptr<Map2D<unsigned int>> map;
	sf::Rect<unsigned int> limits;
	sf::Rect<unsigned int> room;
};

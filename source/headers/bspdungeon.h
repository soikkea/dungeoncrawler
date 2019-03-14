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
};

class BSPDungeon
{
public:
	BSPDungeon(sf::Rect<unsigned int> limits, std::shared_ptr<Map2D<int>> map);
	static std::shared_ptr<BSPDungeon> generateDungeon(int width, int height, int depth);
	void split();
	void connect();
	void generateRoom();
	std::shared_ptr<Map2D<int>> getMap();
protected:
	bool connectNodes(const sf::Rect<unsigned int> & leftLimits, const sf::Rect<unsigned int> & rightLimits);
	std::shared_ptr<BSPDungeon> leftChild;
	std::shared_ptr<BSPDungeon> rightChild;
	std::shared_ptr<Map2D<int>> map;
	sf::Rect<unsigned int> limits;
	sf::Rect<unsigned int> room;
};

#include <queue>
#include <memory>

#include "bspdungeon.h"
#include "level.h"
#include "rng.h"

template<typename T>
Map2D<T>::Map2D(int width, int height, T initValue) :
	width(width),
	height(height),
	map(std::vector<T>(width * height, initValue))
{
}

template<typename T>
void Map2D<T>::setValueAt(int x, int y, T value)
{
	map.at(x + y * width) = value;
}

template<typename T>
T Map2D<T>::getValueAt(int x, int y)
{
	return map.at(x + y * width);
}

BSPDungeon::BSPDungeon(sf::Rect<unsigned int> limits, std::shared_ptr<Map2D<int>> map) :
	leftChild(),
	rightChild(),
	map(map),
	limits(limits),
	room(limits)
{
}

std::shared_ptr<BSPDungeon> BSPDungeon::generateDungeon(int width, int height, int depth)
{
	int mapSize = width * height;
	auto map = std::make_shared<Map2D<int>>(width, height, Level::EMPTY);
	sf::Rect<unsigned int> limits(0, 0, width, height);
	auto root = std::make_shared<BSPDungeon>(limits, map);
	std::queue<std::shared_ptr<BSPDungeon>> nodeQueue;
	std::shared_ptr<BSPDungeon> rootPtr(root);

	// Split the nodes, down to depth
	nodeQueue.push(rootPtr);
	int depthSize = 1;
	for (int i = 0; i < depth; i++)
	{
		int depthSum = 0;
		for (int d = 0; d < depthSize; d++)
		{
			auto & node = nodeQueue.front();
			node->split();
			nodeQueue.push(node->leftChild);
			nodeQueue.push(node->rightChild);
			depthSum += 2;
			nodeQueue.pop();
		}
		depthSize = depthSum;
	}
	
	// Generate rooms for the leafs
	while (!nodeQueue.empty())
	{
		nodeQueue.front()->generateRoom();
		nodeQueue.pop();
	}

	// Connect the nodes depth-first

	return root;
}

void BSPDungeon::split()
{
	// Create children for the node

	// Pick random direction
	// 0 = vertical, 1 = horizontal
	int direction = random::randomInt(1);

	int a, b, splitPos, length, splitLength;

	// Pick random position for split
	if (direction == 0)
	{
		// Vertical split
		length = limits.width;
		splitPos = limits.left;
	}
	else 
	{
		// Horizontal split
		length = limits.height;
		splitPos = limits.top;
	}

	a = (int) (0.45f * length);
	b = (int) (0.55f * length);

	splitLength = random::randomIntBetween(a, b);
	splitPos += splitLength;

	sf::Rect<unsigned int> leftLimits, rightLimits;

	if (direction == 0)
	{
		leftLimits =  sf::Rect<unsigned int>(limits.left, limits.top, splitLength, limits.height);
		rightLimits = sf::Rect<unsigned int>(splitPos, limits.top, length - splitLength, limits.height);
	}
	else
	{
		leftLimits = sf::Rect<unsigned int>(limits.left, limits.top, limits.width, splitLength);
		rightLimits = sf::Rect<unsigned int>(limits.left, splitPos, limits.width, length - splitLength);
	}

	// Create child nodes
	leftChild = std::make_shared<BSPDungeon>(leftLimits, map);
	rightChild = std::make_shared<BSPDungeon>(rightLimits, map);
}

void BSPDungeon::connect()
{
	// Connect the dungeon recursively
	if (leftChild != nullptr)
	{
		leftChild->connect();
		rightChild->connect();

		// Connect the children
	}
	// Do nothing... 
}

void BSPDungeon::generateRoom()
{
	// Generate room inside the node.

	// PLACEHOLDER
	auto right = limits.left + limits.width;
	auto bottom = limits.top + limits.height;
	for (auto x = limits.left; x < right; x++)
	{
		for (auto y = limits.top; y < bottom; y++)
		{
			// Border
			if (
				y == limits.top || y == bottom - 1 ||
				x == limits.left || x == right - 1
				)
			{
				map->setValueAt(x, y, Level::WALL);
			}
			else
			{
				map->setValueAt(x, y, Level::FLOOR);
			}
		}
	}
}

std::shared_ptr<Map2D<int>> BSPDungeon::getMap()
{
	return map;
}

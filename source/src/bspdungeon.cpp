#include <queue>

#include "bspdungeon.h"
#include "level.h"

BSPDungeon::BSPDungeon(sf::Rect<unsigned int> limits, std::shared_ptr<std::vector<int>> map) :
	leftChild(),
	rightChild(),
	map(map),
	room(limits)
{
}

BSPDungeon BSPDungeon::generateDungeon(int width, int height, int depth)
{
	int mapSize = width * height;
	std::shared_ptr<std::vector<int>> map(new std::vector<int>(mapSize, Level::EMPTY));
	sf::Rect<unsigned int> limits(0, 0, width, height);
	auto root = BSPDungeon(limits, map);
	std::queue<std::shared_ptr<BSPDungeon>> nodeQueue;
	int depthSize = 0;
	std::shared_ptr<BSPDungeon> rootPtr(&root);
	nodeQueue.push(rootPtr);
	for (int i = 0; i <= depth; i++)
	{

	}
	// TODO: Return pointer
	return root;
}

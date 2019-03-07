#include <queue>
#include <memory>

#include "bspdungeon.h"
#include "level.h"

BSPDungeon::BSPDungeon(sf::Rect<unsigned int> limits, std::shared_ptr<std::vector<int>> map) :
	leftChild(),
	rightChild(),
	map(map),
	room(limits)
{
}

std::shared_ptr<BSPDungeon> BSPDungeon::generateDungeon(int width, int height, int depth)
{
	int mapSize = width * height;
	std::shared_ptr<std::vector<int>> map(new std::vector<int>(mapSize, Level::EMPTY));
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

	// Connect the nodes depth-first

	return root;
}

void BSPDungeon::split()
{
	// Create children for the node

	// Pick random direction

	// Pick random position for split

	// Create child nodes
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

}

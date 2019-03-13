#include <queue>
#include <memory>

#include "bspdungeon.h"
#include "level.h"
#include "rng.h"
#include "globals.h"

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
	root->connect();

	return root;
}

void BSPDungeon::split()
{
	// Create children for the node

	// Pick random direction
	// 0 = vertical, 1 = horizontal
	enum SplitDir {
		VERTICAL=0,
		HORIZONTAL=1
	};

	SplitDir direction = (SplitDir) random::randomInt(1);

	int a, b, splitPos, length, splitLength;

	// Pick random position for split
	if (direction == VERTICAL)
	{
		// Vertical split
		length = limits.width;
		splitPos = limits.left;
	}
	else if (direction == HORIZONTAL)
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

	if (direction == VERTICAL)
	{
		leftLimits =  sf::Rect<unsigned int>(limits.left, limits.top, splitLength, limits.height);
		rightLimits = sf::Rect<unsigned int>(splitPos, limits.top, length - splitLength, limits.height);
	}
	else if (direction == HORIZONTAL)
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
		if (leftChild->leftChild == nullptr)
		{
			// Children are leaves
			auto & leftRoom = leftChild->room;
			auto & rightRoom = rightChild->room;
			int aTop = leftRoom.top;
			int aLeft = leftRoom.left;
			int aRight = aLeft + leftRoom.width - 1;
			int aBottom = aTop + leftRoom.height - 1;
			int bTop = rightRoom.top;
			int bLeft = rightRoom.left;
			int bRight = bLeft + rightRoom.width - 1;
			int bBottom = bTop + rightRoom.height - 1;
			
			/* Possible connections:
			 * aTop -> bBottom
			 * aLeft -> bRight
			 * aBottom -> bTop
			 * aRight -> bLeft
			 */

			bool top = aTop > bBottom; // a below b
			bool left = aLeft > bRight; // a right of b
			bool bottom = aBottom < bTop; // a above b
			bool right = aRight < bLeft; // a left of b

			int cX, cY, dX, dY;
			bool xOverlap, yOverlap;

			xOverlap = getOverlap(aLeft, aRight, bLeft, bRight, cX, dX);
			yOverlap = getOverlap(aTop, aBottom, bTop, bBottom, cY, dY);

			if (xOverlap)
			{
				int midX = (cX + dX) / 2;
				int cStart, cEnd;
				if (top)
				{
					cStart = bBottom;
					cEnd = aTop;
				}
				else
				{
					cStart = aBottom;
					cEnd = bTop;
				}
				for (int i = cStart; i <= cEnd; i++)
				{
					map->setValueAt(midX, i, Level::FLOOR);
				}
			}
			else if (yOverlap)
			{
				int midY = (cY + dY) / 2;
				int cStart, cEnd;
				if (right)
				{
					cStart = aRight;
					cEnd = bLeft;
				}
				else
				{
					cStart = bRight;
					cEnd = aLeft;
				}
				for (int i = cStart; i <= cEnd; i++)
				{
					map->setValueAt(i, midY, Level::FLOOR);
				}
			}
			else
			{
				// TODO: No overlap
			}

		}
		else
		{
			// Find two rooms, one from each child, that are closest together

			// Connect them

			// TEMP: placeholder implementation, proof of consept
			auto& leftLimits = leftChild->limits;
			auto& rightLimits = rightChild->limits;
			int aTop = leftLimits.top;
			int aLeft = leftLimits.left;
			int aRight = aLeft + leftLimits.width - 1;
			int aBottom = aTop + leftLimits.height - 1;
			int bTop = rightLimits.top;
			int bLeft = rightLimits.left;
			int bRight = bLeft + rightLimits.width - 1;
			int bBottom = bTop + rightLimits.height - 1;

			if (aTop == bTop)
			{
				// A next to B
				for (int y = aTop; y <= aBottom; y++)
				{
					int leftX = aRight;
					int rightX = bLeft;
					bool edgesFound = false;
					while (!edgesFound && (leftX > aLeft && rightX < bRight))
					{
						auto leftXValue = map->getValueAt(leftX, y);
						auto rightXValue = map->getValueAt(rightX, y);
						if (leftXValue != Level::FLOOR) leftX--;
						if (rightXValue != Level::FLOOR) rightX++;
						if (leftXValue == Level::FLOOR && rightXValue == Level::FLOOR) edgesFound = true;
					}
					if (edgesFound)
					{
						for (int i = leftX; i <= rightX; i++)
						{
							map->setValueAt(i, y, Level::FLOOR);
						}
						break;
					}
				}
			}
			else
			{
				// A above B
				for (int x = aLeft; x <= aRight; x++)
				{
					int topY = aBottom;
					int bottomY = bTop;
					bool edgesFound = false;
					while (!edgesFound && (topY > aTop && bottomY < bBottom))
					{
						auto topYValue = map->getValueAt(x, topY);
						auto bottomYValue = map->getValueAt(x, bottomY);
						if (topYValue != Level::FLOOR) topY--;
						if (bottomYValue != Level::FLOOR) bottomY++;
						if (topYValue == Level::FLOOR && bottomYValue == Level::FLOOR) edgesFound = true;
					}
					if (edgesFound)
					{
						for (int i = topY; i <= bottomY; i++)
						{
							map->setValueAt(x, i, Level::FLOOR);
						}
						break;
					}
				}
			}

		}
	}
	// Do nothing... 
}

void BSPDungeon::generateRoom()
{
	// Generate room inside the node.

	const int MAX_ROOM_SIZE = 6;

	auto top = limits.top;
	auto left = limits.left;
	auto right = limits.left + limits.width;
	auto bottom = limits.top + limits.height;

	if (limits.width > MAX_ROOM_SIZE && limits.height > MAX_ROOM_SIZE)
	{
		auto width = random::randomIntBetween(MAX_ROOM_SIZE, limits.width);
		auto height = random::randomIntBetween(MAX_ROOM_SIZE, limits.height);
		top = random::randomIntBetween(top, bottom - height);
		left = random::randomIntBetween(left, right - width);
		right = left + width;
		bottom = top + height;
		room = sf::Rect<unsigned int>(top, left, width, height);
	}

	for (auto x = left; x < right; x++)
	{
		for (auto y = top; y < bottom; y++)
		{
			// Border
			if (
				y == top || y == bottom - 1 ||
				x == left || x == right - 1
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

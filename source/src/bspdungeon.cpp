#include <assert.h>

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

BSPDungeon::BSPDungeon(sf::Rect<unsigned int> limits, std::shared_ptr<Map2D<unsigned int>> map) :
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
	auto map = std::make_shared<Map2D<unsigned int>>(width, height, static_cast<int>(Level::TileType::EMPTY));
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
				assert(map->getValueAt(midX, aTop) != static_cast<int>(Level::TileType::EMPTY));
				assert(map->getValueAt(midX, bBottom) != static_cast<int>(Level::TileType::EMPTY));
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
				createCorridor(midX, cStart - 1, midX, cEnd + 1, 1);
			}
			else if (yOverlap)
			{
				int midY = (cY + dY) / 2;
				assert(map->getValueAt(aRight, midY) != static_cast<int>(Level::TileType::EMPTY));
				assert(map->getValueAt(bLeft, midY) != static_cast<int>(Level::TileType::EMPTY));
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
				createCorridor(cStart - 1, midY, cEnd + 1, midY, 1);
			}
			else
			{
				// TODO: No overlap
				assert(true);
			}

		}
		else
		{
			// Find two rooms, one from each child, that are closest together

			// Connect them

			// TEMP: placeholder implementation, proof of consept
			auto& leftLimits = leftChild->limits;
			auto& rightLimits = rightChild->limits;
			
			connectNodes(leftLimits, rightLimits);

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
		room = sf::Rect<unsigned int>(left, top, width, height);
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
				map->setValueAt(x, y, static_cast<int>(Level::TileType::WALL));
			}
			else
			{
				map->setValueAt(x, y, static_cast<int>(Level::TileType::FLOOR));
			}
		}
	}

	map->rooms.push_back(room);
}

void BSPDungeon::createCorridor(int x1, int y1, int x2, int y2, int width)
{
	// Orientation of the corridor
	bool horizontal = true;
	if (x1 == x2) horizontal = false;
	int top = y1, right = x2, bottom = y2, left = x1;
	if (top > bottom)
	{
		top = y2;
		bottom = y1;
	}
	if (left > right)
	{
		left = x2;
		right = x1;
	}
	if (horizontal)
	{
		top -= width;
		bottom += width;
	}
	else
	{
		left -= width;
		right += width;
	}

	for (int x = left; x <= right; x++)
	{
		for (int y = top; y <= bottom; y++)
		{
			if (x == left || x == right || y == top || y == bottom)
			{
				if (map->getValueAt(x, y) == static_cast<int>(Level::TileType::EMPTY)) map->setValueAt(x, y, static_cast<int>(Level::TileType::WALL));
			}
			else
			{
				map->setValueAt(x, y, static_cast<int>(Level::TileType::FLOOR));
			}
		}
	}


}

std::shared_ptr<Map2D<unsigned int>> BSPDungeon::getMap()
{
	return map;
}

// Connect two non-leaf nodes
bool BSPDungeon::connectNodes(const sf::Rect<unsigned int>& leftLimits, const sf::Rect<unsigned int>& rightLimits)
{
	bool edgesFound = false;
	int aTop = leftLimits.top;
	int aLeft = leftLimits.left;
	int aRight = aLeft + leftLimits.width - 1;
	int aBottom = aTop + leftLimits.height - 1;
	int bTop = rightLimits.top;
	int bLeft = rightLimits.left;
	int bRight = bLeft + rightLimits.width - 1;
	int bBottom = bTop + rightLimits.height - 1;

	const enum Orientation
	{
		HORIZONTAL,
		VERTICAL
	};

	// Is the border between the nodes vertical or horizontal?
	auto orient = (aTop == bTop) ? VERTICAL : HORIZONTAL;

	// Loop over the map either vertically or horizontally
	int iStart = (orient == VERTICAL) ? aTop    : aLeft;
	int iEnd   = (orient == VERTICAL) ? aBottom : aRight;

	// Loop over every row/column, starting from the center outwards
	int aStart = (orient == VERTICAL) ? aRight  : aBottom;
	int aEnd   = (orient == VERTICAL) ? aLeft   : aTop;
	int bStart = (orient == VERTICAL) ? bLeft   : bTop;
	int bEnd   = (orient == VERTICAL) ? bRight  : bBottom;

	int iLeft = (iStart + iEnd) / 2;
	int iRight = iLeft + 1;
	bool toggleIDir = true;

	int i;

	// Start searching the row/column from the middle, and then keep changing the search direction
	while (iLeft >= iStart || iRight <= iEnd)
	{
		if (toggleIDir)
		{
			i = iLeft;
			iLeft--;
			if (iRight <= iEnd) toggleIDir = false;
		}
		else
		{
			i = iRight;
			iRight++;
			if (iLeft >= iStart) toggleIDir = true;
		}
		int a = aStart;
		int b = bStart;
		// Find a row/column that connects the nodes
		while (!edgesFound && (a > aEnd && b < bEnd))
		{
			int aValue, bValue;
			if (orient == VERTICAL)
			{
				aValue = map->getValueAt(a, i);
				bValue = map->getValueAt(b, i);
			}
			else
			{
				aValue = map->getValueAt(i, a);
				bValue = map->getValueAt(i, b);
			}
			if (aValue != static_cast<int>(Level::TileType::FLOOR)) a--;
			if (bValue != static_cast<int>(Level::TileType::FLOOR)) b++;
			if (aValue == static_cast<int>(Level::TileType::FLOOR) && 
				bValue == static_cast<int>(Level::TileType::FLOOR)) edgesFound = true;
		}
		// Create corridor to the selected row/column.
		if (edgesFound)
		{
			if (orient == VERTICAL)
			{
				createCorridor(a, i, b, i, 1);
			}
			else
			{
				createCorridor(i, a, i, b, 1);
			}
			break;
		}
	}

	return edgesFound;
}

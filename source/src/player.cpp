#include "player.h"
#include "level.h"
#include "globals.h"

Player::Player(unsigned int x, unsigned int y) :
	playerCreature(x, y),
	turnOver_(false)
{
	playerCreature.setColor(sf::Color::Blue);
}

bool Player::isTurnOver()
{
	return turnOver_;
}

void Player::endTurn()
{
	turnOver_ = true;
}

void Player::resetTurn()
{
	turnOver_ = false;
}

void Player::handleMoveInput(Direction direction, Level & level)
{
	auto retVal = playerCreature.moveStep(direction, level);
	if (retVal) {
		endTurn();
		return;
	}

	auto newPos = playerCreature.getTilePos() + getDirectionUnitVector(direction);

	auto creatures = level.getCreatures();
	for (auto creature_ptr : creatures)
	{
		if (creature_ptr->getTilePos() == newPos) {
			playerCreature.attackCreature(*creature_ptr);
			endTurn();
			return;
		}
	}
}

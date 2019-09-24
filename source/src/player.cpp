#include "player.h"
#include "level.h"
#include "globals.h"

Player::Player(unsigned int x, unsigned int y) :
	Creature(x, y),
	turnOver_(false)
{
	setColor(sf::Color::Blue);
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

bool Player::moveStep(Direction direction, const Level & level)
{
	auto retVal = this->Sprite::moveStep(direction, level);
	if (retVal) {
		endTurn();
	}
	return retVal;
}

void Player::moveInput(Direction direction, Level & level)
{
	auto retVal = moveStep(direction, level);
	if (retVal) return;

	auto newPos = getTilePos() + getDirectionUnitVector(direction);

	auto creatures = level.getCreatures();
	for (auto creature_ptr : creatures)
	{
		if (creature_ptr->getTilePos() == newPos) {
			attackCreature(*creature_ptr);
			endTurn();
			return;
		}
	}
}

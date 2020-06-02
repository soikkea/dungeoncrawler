#include "player.h"
#include "level.h"
#include "globals.h"
#include "creature.h"
#include "item.h"

Player::Player(int x, int y) :
	playerCreature(x, y),
	turnOver_(false)
{
	playerCreature.setColor(sf::Color::Blue);
	playerCreature.setName("Player");
	playerCreature.equipWeapon(std::make_unique<Weapon>("Sword", 1, 2, 100));
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
	auto newPos = playerCreature.getTilePos() + getDirectionUnitVector(direction);

	auto retVal = playerCreature.moveStep(direction, level);
	if (retVal) {
		level.useItemAt(newPos.x, newPos.y, playerCreature);
		endTurn();
		return;
	}

	// Collision detection
	for (auto& creature_ptr : level.getCreatures())
	{
		if (creature_ptr->getTilePos() == newPos) {
			playerCreature.attackCreature(*creature_ptr);
			endTurn();
			return;
		}
	}
}

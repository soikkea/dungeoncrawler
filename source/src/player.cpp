#include "player.h"
#include "level.h"
#include "globals.h"
#include "creature.h"
#include "item.h"
#include "hud.h"

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
	return turnOver_ || !playerCreature.canMove();
}

void Player::endTurn()
{
	turnOver_ = true;
}

void Player::resetTurn()
{
	turnOver_ = false;
	playerCreature.startTurn();
}

void Player::handleMoveInput(Direction direction, Level & level)
{
	auto newPos = playerCreature.getTilePos() + getDirectionUnitVector(direction);

	auto retVal = playerCreature.moveStep(direction, level);
	if (retVal) {
		return;
	}
	else if (level.tileHasItem(newPos.x, newPos.y) >= 0) {
		if (level.pickUpItemAt(newPos.x, newPos.y, playerCreature)) {
			// Picked up item
			auto text = playerCreature.getName() + " picked up " + playerCreature.getInventory().back()->getName() + "\n";
			Hud::actionLog.push_back(text);

			playerCreature.moveStep(direction, level);
		}
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

void Player::resetPlayer()
{
	playerCreature = Creature(0, 0);
	playerCreature.getInventory().clear();
	playerCreature.setColor(sf::Color::Blue);
	playerCreature.setName("Player");
	playerCreature.equipWeapon(std::make_unique<Weapon>("Sword", 1, 2, 100));
}
#include "creature.h"
#include "rng.h"

Creature::Creature(unsigned int x, unsigned int y) :
	_maxHitPoints(0),
	_hitPoints(0)
{
	setTilePos(x, y);
	setColor(sf::Color::Green);
}

Creature::~Creature()
{
}

const int Creature::getHitPoints() const
{
	return _hitPoints;
}

const int Creature::getMaxHitPoints() const
{
	return _maxHitPoints;
}

void Creature::gainHitpoints(int amount)
{
	_hitPoints += amount;
}

bool Creature::isAlive() const
{
	return _hitPoints > 0;
}

void Creature::update(Level & level)
{
	int dir = random::randomInt(3);
	for (int i = 0; i < 4; i++) {
		auto didMove = moveStep(static_cast<Direction>(dir), level);
		if (didMove) {
			break;
		}
		dir = (dir + 1) % 4;
	}
}

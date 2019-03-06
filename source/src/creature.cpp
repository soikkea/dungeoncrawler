#include "creature.h"

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

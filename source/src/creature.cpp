#include <sstream>
#include "creature.h"
#include "rng.h"
#include "globals.h"
#include "player.h"
#include "hud.h"
#include "level.h"

Creature::Creature(unsigned int x, unsigned int y) :
	_maxHitPoints(10),
	_hitPoints(10),
	_name("Creature"),
	_sightLine(sf::Lines, 2)
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

const std::string Creature::getName() const
{
	return _name;
}

void Creature::setName(std::string name)
{
	_name = name;
}

void Creature::update(Level & level, Player& player)
{
	if (!isAlive()) return;

	_seesPlayer = level.getLineOfSight(getTilePos(), player.playerCreature.getTilePos());

	_sightLine[1].position = player.playerCreature.getWorldCenter();

	if (intDistance(getTilePos(), player.playerCreature.getTilePos()) <= 1) {
		return;
	}

	int dir = random::randomInt(3);
	for (int i = 0; i < 4; i++) {
		auto didMove = moveStep(static_cast<Direction>(dir), level);
		if (didMove) {
			break;
		}
		dir = (dir + 1) % 4;
	}

	_sightLine[0].position = getWorldCenter();
}

void Creature::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	Sprite::draw(target, states);

	if (_seesPlayer) {
		target.draw(_sightLine, states);
	}
}

void Creature::attackCreature(Creature & target)
{
	if (intDistance(getTilePos(), target.getTilePos()) <= 1) {
		target.gainHitpoints(-1);
	}

	std::ostringstream oss;
	oss << _name << " attacked " << target.getName() << ", dealing 1 damage." << std::endl;
	Hud::actionLog.push_back(oss.str());

	if (!target.isAlive()) {
		std::ostringstream oss;
		oss << target.getName() << " was killed!" << std::endl;
		Hud::actionLog.push_back(oss.str());
	}
}

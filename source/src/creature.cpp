#include <sstream>
#include "creature.h"
#include "rng.h"
#include "globals.h"
#include "player.h"
#include "hud.h"
#include "level.h"
#include "item.h"

Creature::Creature(int x, int y) :
	_maxHitPoints(10),
	_hitPoints(10),
	_name("Creature"),
	_sightLine(sf::Lines, 2),
	_level(1),
	_experience(0),
	_experienceWorth(50),
	_experienceReqToNextLevel(100),
	_seesPlayer(false),
	_equippedWeapon(std::make_unique<Weapon>("Weak Fists", 0, 1, 50))
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
	if (_hitPoints > _maxHitPoints) {
		_hitPoints = _maxHitPoints;
	}
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

const int Creature::getLevel() const
{
	return _level;
}

const int Creature::getExperience() const
{
	return _experience;
}

const int Creature::getExperienceWorth() const
{
	return _experienceWorth;
}

const int Creature::getExpRequiredToNextLevel() const
{
	return _experienceReqToNextLevel;
}

void Creature::gainExperience(int amount)
{
	_experience += amount;
	if (_experience >= _experienceReqToNextLevel) {
		gainLevel(1);
	}
}

void Creature::gainLevel(int amount)
{
	for (int i = 0; i < amount; i++) {
		_level++;
		_experienceReqToNextLevel = (int)(_experienceReqToNextLevel * 2.1f);
	}
	gainHitpoints(_maxHitPoints);

	std::ostringstream oss;
	oss << _name << " leveled up!" << std::endl;
	Hud::actionLog.push_back(oss.str());
}

void Creature::equipWeapon(std::unique_ptr<Weapon> weapon)
{
	_equippedWeapon = std::move(weapon);
}

std::vector<std::unique_ptr<Item>>& Creature::getInventory()
{
	return _inventory;
}

bool Creature::addItem(std::unique_ptr<Item>& item)
{
	// TODO: No magic numbers
	if (_inventory.size() >= 5)
		return false;

	_inventory.push_back(std::move(item));

	return true;
}

const int Creature::getDamage() const
{
	return _equippedWeapon->getDamage();
}

void Creature::update(Level & level, Player& player)
{
	if (!isAlive()) return;

	_seesPlayer = level.getLineOfSight(getTilePos(), player.playerCreature.getTilePos());

	_sightLine[1].position = player.playerCreature.getWorldCenter();

	if (tileDistance(getTilePos(), player.playerCreature.getTilePos()) == 1) {
		attackCreature(player.playerCreature);
		return;
	}

	if (intDistance(getTilePos(), player.playerCreature.getTilePos()) <= 1) {
		return;
	}

	int dir = rng::randomInt(3);
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

	drawHealth(target, states);
}

void Creature::drawHealth(sf::RenderTarget& target, sf::RenderStates states) const
{
	const auto width = (float)globals::TILE_SIZE * (_hitPoints / (float)_maxHitPoints);
	sf::RectangleShape healthBar(sf::Vector2f(width, 5.f));
	healthBar.setFillColor(sf::Color::Red);
	
	states.transform *= getTransform();

	target.draw(healthBar, states);
}

void Creature::attackCreature(Creature & target)
{
	int damage = 0;
	if (intDistance(getTilePos(), target.getTilePos()) <= 1) {
		bool hit = _equippedWeapon->onUse(target);
		if (hit)
			damage = getDamage();
	}

	std::ostringstream oss;
	oss << _name << " attacked " << target.getName() << ", dealing " << damage << " damage." << std::endl;
	Hud::actionLog.push_back(oss.str());

	if (!target.isAlive()) {
		std::ostringstream oss;
		oss << target.getName() << " was killed!" << std::endl;
		Hud::actionLog.push_back(oss.str());

		std::ostringstream oss2;
		oss2 << _name << " gained " << target.getExperienceWorth() << " experience." << std::endl;
		Hud::actionLog.push_back(oss2.str());
		gainExperience(target.getExperienceWorth());
	}
}

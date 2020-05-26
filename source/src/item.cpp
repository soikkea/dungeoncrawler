#include "item.h"
#include "creature.h"
#include "rng.h"

Item::Item(std::string name, int weight) :
	_name(name),
	_weight(weight)
{
}

HealthPotion::HealthPotion() :
	Item("HealthPotion", 1)
{
}

bool HealthPotion::onInteract(Creature& creature)
{
	// TODO: Pick up item
	return this->onUse(creature);
}

bool HealthPotion::onUse(Creature& target)
{
	target.gainHitpoints(this->_healthAmount);
	return true;
}

Weapon::Weapon(std::string name, int weight, int damage, int baseHitChance) :
	Item(name, weight),
	_damage(damage),
	_baseHitChance(baseHitChance)
{
	if (_baseHitChance < 0)
		_baseHitChance = 0;
	if (_baseHitChance > 100)
		_baseHitChance = 100;
}

bool Weapon::onInteract(Creature& creature)
{
	// TODO: Equip the weapon
	return false;
}

bool Weapon::onUse(Creature& target)
{
	int hitThrow = rng::randomIntBetween(1, 100);
	if (hitThrow > _baseHitChance)
		return false;

	target.gainHitpoints(-_damage);

	return true;
}

const int Weapon::getDamage() const
{
	return _damage;
}


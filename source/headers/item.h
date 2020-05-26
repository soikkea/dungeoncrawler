#pragma once

#include <string>

class Creature;

class Item
{
public:
	Item(std::string name, int weight);

	virtual bool onInteract(Creature& creature) { return false; }
	virtual bool onUse(Creature& target) { return false; }

private:
	std::string _name;
	int _weight;
};

class HealthPotion : public Item
{
public:
	HealthPotion();

	bool onInteract(Creature& creature) override;
	bool onUse(Creature& target) override;

private:
	int _healthAmount = 10;
};

class Weapon : public Item
{
public:
	Weapon(std::string name, int weight, int damage, int baseHitChance);

	bool onInteract(Creature& creature) override;
	bool onUse(Creature& target) override;

	const int getDamage() const;

private:
	int _damage;
	int _baseHitChance;
};
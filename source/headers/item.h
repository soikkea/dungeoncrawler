#pragma once

#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

class Creature;
class Sprite;

class Item : public sf::Drawable, public sf::Transformable
{
public:
	Item(std::string name, int weight);

	virtual bool onUse(Creature& target) { return false; }

	const sf::Vector2i getTilePos() const;
	void setTilePos(int x, int y);

	const std::string getName() const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	std::string _name;
	int _weight;
	std::unique_ptr<Sprite> _sprite;
};

class HealthPotion : public Item
{
public:
	HealthPotion();

	bool onUse(Creature& target) override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	int _healthAmount = 10;
};

class Weapon : public Item
{
public:
	Weapon(std::string name, int weight, int damage, int baseHitChance);

	bool onUse(Creature& target) override;

	const int getDamage() const;

protected:
	int _damage;
	int _baseHitChance;
};
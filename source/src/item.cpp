#include "item.h"
#include "creature.h"
#include "rng.h"
#include "sprite.h"

Item::Item(std::string name, int weight) :
	_name(name),
	_weight(weight),
	_sprite(std::make_unique<Sprite>())
{
}

const sf::Vector2i Item::getTilePos() const
{
	return this->_sprite->getTilePos();
}

void Item::setTilePos(int x, int y)
{
	this->_sprite->setTilePos(x, y);
}

const std::string Item::getName() const
{
	return _name;
}

void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	this->_sprite->draw(target, states);
}

HealthPotion::HealthPotion() :
	Item("Health Potion", 1)
{
}

bool HealthPotion::onUse(Creature& target)
{
	target.gainHitpoints(this->_healthAmount);
	return true;
}

void HealthPotion::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::CircleShape circle(globals::TILE_SIZE * 0.5);
	circle.setFillColor(sf::Color::Red);

	states.transform *= this->_sprite->getTransform();

	target.draw(circle, states);
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


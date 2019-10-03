#pragma once

#include "sprite.h"

class Player;

class Creature : public Sprite
{
public:
	Creature(unsigned int x, unsigned int y);
	virtual ~Creature();

	const int getHitPoints() const;
	const int getMaxHitPoints() const;
	void gainHitpoints(int amount);
	virtual bool isAlive() const;
	const std::string getName() const;
	void setName(std::string name);

	void update(Level& level, Player& player);

	virtual void attackCreature(Creature& target);
protected:
	int _maxHitPoints;
	int _hitPoints;
	std::string _name;
	bool _seesPlayer;
};
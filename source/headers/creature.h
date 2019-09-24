#pragma once

#include "sprite.h"

class Creature : public Sprite
{
public:
	Creature(unsigned int x, unsigned int y);
	virtual ~Creature();

	const int getHitPoints() const;
	const int getMaxHitPoints() const;
	void gainHitpoints(int amount);
	virtual bool isAlive() const;

	void update(Level& level);
protected:
	int _maxHitPoints;
	int _hitPoints;
};
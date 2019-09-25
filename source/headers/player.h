#pragma once

#include "globals.h"
#include "creature.h"

class Level;

class Player
{
public:
	Player(unsigned int x, unsigned int y);

	bool isTurnOver();
	void endTurn();
	void resetTurn();

	void handleMoveInput(Direction direction, Level& level);

	Creature playerCreature;

private:
	bool turnOver_;
};
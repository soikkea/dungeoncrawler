#pragma once

#include "globals.h"
#include "creature.h"

class Level;

class Player
{
public:
	Player(int x, int y);

	bool isTurnOver();
	void endTurn();
	void resetTurn();

	void handleMoveInput(Direction direction, Level& level);

	void resetPlayer();

	Creature playerCreature;

private:
	bool turnOver_;
};
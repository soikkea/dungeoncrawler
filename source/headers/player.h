#pragma once

#include "creature.h"

class Player : public Creature
{
public:
	Player(unsigned int x, unsigned int y);

	bool isTurnOver();
	void endTurn();
	void resetTurn();

	bool moveStep(Direction direction, const Level& level);
	void handleMoveInput(Direction direction, Level& level);
private:
	bool turnOver_;
};
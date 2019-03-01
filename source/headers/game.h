#pragma once
#include <SFML/Graphics.hpp>
#include "creature.h"
#include "level.h"

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw();
	void update(float elapsedTime);

	sf::RenderWindow* m_window;
	Creature m_player;
	Level _level;
};
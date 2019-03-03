#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"
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
	Player m_player;
	Level _level;
};
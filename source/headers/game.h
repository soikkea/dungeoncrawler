#pragma once
#include <SFML/Graphics.hpp>
#include "sprite.h"
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
	Sprite m_player;
	Level _level;
};
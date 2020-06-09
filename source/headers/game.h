#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"
#include "level.h"
#include "hud.h"


class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw();
	void update(float elapsedTime);

	enum
	{
		MODE_GAME,
		MODE_INVENTORY,
		MODE_SKILLS,
		MODE_MENU
	};

	int _gameMode = MODE_GAME;
	sf::RenderWindow* m_window;
	Player m_player;
	Level _level;
	sf::View* _gameView;
	Hud _hud;
};
#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "player.h"
#include "level.h"
#include "hud.h"


class Game {
public:
	Game();
	~Game();

	bool isInProgress();
private:
	void startNewGame();
	void initializeNewLevel();
	void gameLoop();
	void draw();
	void update(float elapsedTime);
	bool handleMenuEvent(sf::Event& event);
	bool handleGameEvent(sf::Event& event);
	bool handleInventoryEvent(sf::Event& event);
	bool handleSkillsEvent(sf::Event& event);

	enum
	{
		MODE_GAME,
		MODE_INVENTORY,
		MODE_SKILLS,
		MODE_MENU
	};

	enum class GameStatus
	{
		NOT_STARTED,
		STARTED,
		PLAYER_DIED
	};

	int _gameMode = MODE_GAME;
	GameStatus _gameStatus = GameStatus::NOT_STARTED;
	std::unique_ptr<sf::RenderWindow> _window;
	Player m_player;
	Level _level;
	std::unique_ptr<sf::View> _gameView;
	std::unique_ptr<Hud> _hud;
};
#include <SFML/Graphics.hpp>

#include "game.h"
#include "globals.h"
#include "assets.h"

Game::Game() : 
	m_player(0, 0)
{
	// Initialize RendererWindow
	m_window = new sf::RenderWindow(sf::VideoMode(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT), "Dungeoncrawler");

	_gameView = new sf::View(sf::FloatRect(0.f, 0.f, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT * 0.8f));

	m_window->setView(*_gameView);

	Assets::get().LoadTextures();

	_hud = std::move(Hud(
		sf::FloatRect(0.f, 0.f, globals::SCREEN_WIDTH * 0.8f, globals::SCREEN_HEIGHT * 0.2f),
		sf::FloatRect(0.f, 0.8f, 1.f, 0.2f),
		sf::FloatRect(0.f, 0.f, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT),
		sf::FloatRect(0.8f, 0.8f, 0.2f, 0.2f)
	));

	// PLACEHOLDER: init level
	_level = Level();
	_level.tmpInit();
	auto playerPos = _level.getPlayerStartingPos();

	// PLACEHOLDER: init player
	m_player.playerCreature.setTilePos(playerPos.x, playerPos.y);

	this->gameLoop();
}

Game::~Game() {
	delete m_window;
	delete _gameView;
}

void Game::gameLoop() {

	sf::Clock clock;

	float last_update_time = clock.getElapsedTime().asSeconds();

	while (m_window->isOpen()) {
		sf::Event event;
		while (m_window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				m_window->close();
			// Controls
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					m_window->close();
					break;
				default:
					break;
				}
				switch (_gameMode)
				{
				case MODE_GAME:
					handleGameEvent(event);
					break;
				case MODE_INVENTORY:
					handleInventoryEvent(event);
					break;
				case MODE_SKILLS:
					handleSkillsEvent(event);
					break;
				default:
					break;
				}
			}
		}

		float current_time = clock.getElapsedTime().asSeconds();
		float elapsed_time = current_time - last_update_time;
		last_update_time = current_time;
		update(elapsed_time);
		draw();
	}
}

bool Game::handleGameEvent(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code)
		{
		case sf::Keyboard::W:
			m_player.handleMoveInput(Direction::NORTH, _level);
			return true;
		case sf::Keyboard::D:
			m_player.handleMoveInput(Direction::EAST, _level);
			return true;
		case sf::Keyboard::S:
			m_player.handleMoveInput(Direction::SOUTH, _level);
			return true;
		case sf::Keyboard::A:
			m_player.handleMoveInput(Direction::WEST, _level);
			return true;
		case sf::Keyboard::I:
			_gameMode = MODE_INVENTORY;
			return true;
		case sf::Keyboard::C:
			_gameMode = MODE_SKILLS;
			return true;
		default:
			break;
		}
	}
	return false;
}

bool Game::handleInventoryEvent(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code)
		{
		case sf::Keyboard::Num1:
			m_player.playerCreature.useInventoryItem(0);
			return true;
		case sf::Keyboard::Num2:
			m_player.playerCreature.useInventoryItem(1);
			return true;
		case sf::Keyboard::Num3:
			m_player.playerCreature.useInventoryItem(2);
			return true;
		case sf::Keyboard::Num4:
			m_player.playerCreature.useInventoryItem(3);
			return true;
		case sf::Keyboard::Num5:
			m_player.playerCreature.useInventoryItem(4);
			return true;
		case sf::Keyboard::I:
			_gameMode = MODE_GAME;
			return true;
		default:
			break;
		}
	}
	return false;
}

bool Game::handleSkillsEvent(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code)
		{
		case sf::Keyboard::C:
			_gameMode = MODE_GAME;
			return true;
		default:
			break;
		}
	}
	return false;
}

void Game::draw() {
	m_window->clear();

	// Center window on player
	_gameView->setCenter(m_player.playerCreature.getWorldCenter());
	_gameView->setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 0.8f));
	m_window->setView(*_gameView);

	m_window->draw(_level);
	m_window->draw(m_player.playerCreature);

	_hud.draw(*m_window, _level);

	if (_gameMode == MODE_INVENTORY) {
		_hud.drawInventory(*m_window, m_player);
	}
	else if (_gameMode == MODE_SKILLS)
	{
		_hud.drawSkills(*m_window, m_player);
	}

	m_window->display();
}

void Game::update(float elapsedTime) {
	char buffer[100];
	snprintf(buffer, sizeof(buffer), "Dungeoncrawler, FPS=%3.2f", 1.0f/elapsedTime);
	std::string title = buffer;
	m_window->setTitle(title);

	if (m_player.isTurnOver()) {
		_level.update(m_player);

		m_player.resetTurn();
	}

	_hud.update(elapsedTime, m_player);
}
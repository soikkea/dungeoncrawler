#include <SFML/Graphics.hpp>

#include "game.h"

Game::Game() : 
	m_player(0, 0)
{
	// Initialize RendererWindow
	m_window = new sf::RenderWindow(sf::VideoMode(800, 600), "Dungeoncrawler");

	// PLACEHOLDER: init level
	_level = Level();
	_level.tmpInit();
	auto playerPos = _level.getPlayerStartingPos();

	// PLACEHOLDER: init player
	m_player.setTilePos(playerPos.x, playerPos.y);

	this->gameLoop();
}

Game::~Game() {
	delete m_window;
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
				case sf::Keyboard::W:
					m_player.moveStep(NORTH, _level);
					break;
				case sf::Keyboard::D:
					m_player.moveStep(EAST, _level);
					break;
				case sf::Keyboard::S:
					m_player.moveStep(SOUTH, _level);
					break;
				case sf::Keyboard::A:
					m_player.moveStep(WEST, _level);
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

void Game::draw() {
	m_window->clear();

	// Center window on player
	sf::View view = m_window->getDefaultView();
	view.setCenter(m_player.getWorldCenter());
	m_window->setView(view);

	m_window->draw(_level);
	m_window->draw(m_player);
	m_window->display();
}

void Game::update(float elapsedTime) {
	char buffer[100];
	snprintf(buffer, sizeof(buffer), "Dungeoncrawler, FPS=%3.2f", 1.0f/elapsedTime);
	std::string title = buffer;
	m_window->setTitle(title);

	if (m_player.isTurnOver()) {
		_level.update();

		m_player.resetTurn();
	}
}
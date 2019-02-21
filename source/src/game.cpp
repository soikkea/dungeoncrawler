#include <SFML/Graphics.hpp>

#include "game.h"

Game::Game() {
	// Initialize RendererWindow
	m_window = new sf::RenderWindow(sf::VideoMode(800, 600), "Dungeoncrawler");

	// PLACEHOLDER: init player
	m_player = Sprite();

	this->gameLoop();
}

Game::~Game() {
	delete m_window;
}

void Game::gameLoop() {

	while (m_window->isOpen()) {
		sf::Event event;
		while (m_window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				m_window->close();
		}

		// Todo: get a clock for time
		update(0);
		draw();
	}
}

void Game::draw() {
	m_window->clear();
	m_window->draw(m_player);
	m_window->display();
}

void Game::update(float elapsedTime) {

}
#include <SFML/Graphics.hpp>

#include "game.h"

Game::Game() {
	this->gameLoop();
}

Game::~Game() {

}

void Game::gameLoop() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Dungeoncrawler");

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.display();
	}
}

void Game::draw() {

}

void Game::update(float elapsedTime) {

}
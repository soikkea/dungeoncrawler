#pragma once

#include <SFML/Graphics.hpp>

class Player;
class Level;

class Hud {
public:
	Hud();
	Hud(const sf::FloatRect& infoViewSize, const sf::FloatRect& infoViewPort, 
		const sf::FloatRect& miniMapViewSize, const sf::FloatRect& miniMapViewPort);
	~Hud();
	void draw(sf::RenderWindow& window, const Level& level);
	void update(float elapsedTime, const Player& player);
private:
	sf::View _infoView;
	sf::View _miniMapView;
	sf::FloatRect _infoViewPort;
	sf::FloatRect _miniMapViewPort;
};
#pragma once

#include <list>

#include <SFML/Graphics.hpp>

class Player;
class Level;

class Hud {
public:
	Hud();
	Hud(const sf::FloatRect& infoViewSize, const sf::FloatRect& infoViewPort, 
		const sf::FloatRect& miniMapViewSize, const sf::FloatRect& miniMapViewPort);
	Hud(const Hud& otherHud);
	~Hud();
	void draw(sf::RenderWindow& window, const Level& level);
	void update(float elapsedTime, const Player& player);

	std::string getActionLogString();

	Hud& operator=(const Hud& other);

	static std::list<std::string> actionLog;
private:
	sf::View _infoView;
	sf::View _miniMapView;
	sf::FloatRect _infoViewPort;
	sf::FloatRect _miniMapViewPort;
	sf::Font _font;
	sf::Text _text;
	sf::Text _logText;
};
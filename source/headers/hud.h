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
	void drawInventory(sf::RenderWindow& window, Player& player);
	void update(float elapsedTime, const Player& player);

	std::string getActionLogString();

	Hud& operator=(const Hud& other);

	sf::Text createText(std::string text, float xPos, float yPos, int size);

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
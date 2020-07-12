#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include "hudbutton.h"

class Player;
class Level;

class Hud {
public:
	Hud();
	Hud(const sf::FloatRect& infoViewSize, const sf::FloatRect& infoViewPort, 
		const sf::FloatRect& miniMapViewSize, const sf::FloatRect& miniMapViewPort);
	Hud(Hud& otherHud);
	Hud(Hud&& otherHud) noexcept;
	~Hud();
	void draw(sf::RenderWindow& window, const Level& level);
	void drawInventory(sf::RenderWindow& window, Player& player);
	void drawSkills(sf::RenderWindow& window, Player& player);
	void drawMenu(sf::RenderWindow& window, Player& player);
	void update(float elapsedTime, const Player& player);

	std::string const * getClickedButton(int x, int y);

	std::string getActionLogString();

	Hud& operator=(Hud& other);
	Hud& operator=(Hud&& other) noexcept;

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
	std::map<std::string, std::unique_ptr<HudButton>> _skillsButtons;
	std::map<std::string, std::unique_ptr<HudButton>> _menuButtons;
};
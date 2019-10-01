#include <sstream>
#include <stdexcept>

#include "hud.h"
#include "player.h"
#include "level.h"


std::list<std::string> Hud::actionLog = std::list<std::string>();


Hud::Hud()
{
}

Hud::Hud(const sf::FloatRect & infoViewSize, const sf::FloatRect & infoViewPort, 
	const sf::FloatRect & miniMapViewSize, const sf::FloatRect & miniMapViewPort) :
	_infoView(infoViewSize),
	_miniMapView(miniMapViewSize),
	_infoViewPort(infoViewPort),
	_miniMapViewPort(miniMapViewPort)
{
	_miniMapView.zoom(1.2f);

	_infoView.setViewport(_infoViewPort);
	_miniMapView.setViewport(_miniMapViewPort);

	_font = sf::Font();

	if (!_font.loadFromFile("../resources/fonts/Roboto-Regular.ttf")) {
		throw std::runtime_error("Could not load fonts");
	}

	_text = sf::Text();
	_text.setFont(_font);
	_text.setFillColor(sf::Color::White);
	_text.setCharacterSize(16);
	_text.setPosition(0.f, 0.f);

	_logText = sf::Text();
	_logText.setFont(_font);
	_logText.setFillColor(sf::Color::White);
	_logText.setCharacterSize(16);
	_logText.setPosition(150.f, 0.f);

	actionLog.push_back("This is a test");
}

Hud::Hud(const Hud & otherHud) :
	_infoView(otherHud._infoView),
	_miniMapView(otherHud._miniMapView),
	_infoViewPort(otherHud._infoViewPort),
	_miniMapViewPort(otherHud._miniMapViewPort),
	_font(otherHud._font),
	_text(otherHud._text),
	_logText(otherHud._logText)
{
	_text.setFont(_font);
	_logText.setFont(_font);
}

Hud::~Hud()
{
}

void Hud::draw(sf::RenderWindow & window, const Level& level)
{
	window.setView(_infoView);

	window.draw(_text);

	window.draw(_logText);

	_miniMapView.setViewport(_miniMapViewPort);
	window.setView(_miniMapView);
	window.draw(level);
}

void Hud::update(float elapsedTime, const Player& player)
{
	_miniMapView.setCenter(player.playerCreature.getWorldCenter());

	std::ostringstream playerHealth;
	playerHealth.setf(std::ios::fixed);
	playerHealth << "Health: " << player.playerCreature.getHitPoints() << "/" << player.playerCreature.getMaxHitPoints() << std::endl;
	_text.setString(playerHealth.str());

	_logText.setString(getActionLogString());
}

std::string Hud::getActionLogString()
{
	while (actionLog.size() > 10) {
		actionLog.pop_front();
	}
	std::string logString;

	for (auto line : actionLog) {
		logString = logString + line;
	}

	return logString;
}

Hud & Hud::operator=(const Hud & other)
{
	if (&other == this)
		return *this;
	_infoView = other._infoView;
	_miniMapView = other._miniMapView;
	_infoViewPort = other._infoViewPort;
	_miniMapViewPort = other._miniMapViewPort;
	_font = other._font;
	_text = other._text;
	_text.setFont(_font);
	_logText = other._logText;
	_logText.setFont(_font);
	return *this;
}

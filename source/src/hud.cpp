#include "hud.h"
#include "player.h"
#include "level.h"

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
}

Hud::~Hud()
{
}

void Hud::draw(sf::RenderWindow & window, const Level& level)
{
	window.setView(_infoView);

	// TODO: draw infoView

	_miniMapView.setViewport(_miniMapViewPort);
	window.setView(_miniMapView);
	window.draw(level);
}

void Hud::update(float elapsedTime, const Player& player)
{
	_miniMapView.setCenter(player.playerCreature.getWorldCenter());
}

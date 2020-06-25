#include <sstream>
#include <stdexcept>
#include <utility>

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

	if (!_font.loadFromFile("./resources/fonts/Roboto-Regular.ttf")) {
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

	auto addIncrementButton = [&](sf::Vector2f position, std::string name)
	{
		_skillsButtons[name] = std::make_unique<IncrementButton>(position, name);
	};

	addIncrementButton(sf::Vector2f(320.f, 92.f), "increment_attr_agility");
	addIncrementButton(sf::Vector2f(320.f, 112.f), "increment_attr_constitution");
	addIncrementButton(sf::Vector2f(320.f, 132.f), "increment_attr_strength");
	addIncrementButton(sf::Vector2f(320.f, 172.f), "increment_skill_melee");
}

Hud::Hud(Hud & otherHud) :
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

Hud::Hud(Hud&& otherHud) noexcept :
	_infoView(std::move(otherHud._infoView)),
	_miniMapView(std::move(otherHud._miniMapView)),
	_infoViewPort(std::move(otherHud._infoViewPort)),
	_miniMapViewPort(std::move(otherHud._miniMapViewPort)),
	_font(std::move(otherHud._font)),
	_text(std::move(otherHud._text)),
	_logText(std::move(otherHud._logText)),
	_skillsButtons(std::move(otherHud._skillsButtons))
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

void Hud::drawInventory(sf::RenderWindow& window, Player& player)
{
	auto view = window.getDefaultView();

	auto viewSize = view.getSize();

	window.setView(view);

	auto backgroundSize = viewSize * 0.9f;

	sf::RectangleShape background(backgroundSize);

	background.setFillColor(sf::Color::Black);
	background.setOutlineThickness(5);
	background.setOutlineColor(sf::Color::Blue);
	background.move((viewSize - backgroundSize) * 0.5f);

	window.draw(background);

	auto title = createText("INVENTORY", 150.f, 50.f, 18);
	window.draw(title);

	auto& playerInventory = player.playerCreature.getInventory();

	int i = 0;
	for each (auto& itemPtr in playerInventory)
	{
		auto itemText = createText(std::to_string(i + 1) + ". " + itemPtr->getName(), 150.f, 100.f + i * 50.f, 16);
		window.draw(itemText);
		i++;
	}
}

void Hud::drawSkills(sf::RenderWindow& window, Player& player)
{
	auto view = window.getDefaultView();

	auto viewSize = view.getSize();

	window.setView(view);

	auto backgroundSize = viewSize * 0.9f;

	sf::RectangleShape background(backgroundSize);

	background.setFillColor(sf::Color::Black);
	background.setOutlineThickness(5);
	background.setOutlineColor(sf::Color::Blue);
	background.move((viewSize - backgroundSize) * 0.5f);

	window.draw(background);

	auto title = createText("Character Sheet", 150.f, 50.f, 18);
	window.draw(title);

	auto& playerSkillSet = player.playerCreature.getSkillSet();

	auto attributeTitle = createText("Attributes", 150.f, 70.f, 17);
	window.draw(attributeTitle);

	sf::Text text;

	text = createText(std::to_string(playerSkillSet.attributePoints)+ " points", 300.f, 70.f, 17);
	window.draw(text);

	text = createText("Agility", 150.f, 90.f, 16);
	window.draw(text);

	text = createText(std::to_string(playerSkillSet.attributes[Attribute::AGILITY]), 300.f, 90.f, 16);
	window.draw(text);

	text = createText("Constitution", 150.f, 110.f, 16);
	window.draw(text);

	text = createText(std::to_string(playerSkillSet.attributes[Attribute::CONSTITUTION]), 300.f, 110.f, 16);
	window.draw(text);

	text = createText("Strength", 150.f, 130.f, 16);
	window.draw(text);

	text = createText(std::to_string(playerSkillSet.attributes[Attribute::STRENGTH]), 300.f, 130.f, 16);
	window.draw(text);

	auto skillsTitle = createText("Skills", 150.f, 150.f, 17);
	window.draw(skillsTitle);

	text = createText(std::to_string(playerSkillSet.skillPoints) + " points", 300.f, 150.f, 17);
	window.draw(text);

	text = createText("Melee", 150.f, 170.f, 16);
	window.draw(text);

	text = createText(std::to_string(playerSkillSet.skills[Skill::MELEE]), 300.f, 170.f, 16);
	window.draw(text);

	for each (auto& pair in _skillsButtons)
	{
		auto& name = pair.first;
		auto& button = *(pair.second.get());
		if (name.find("increment_attr_") == 0) {
			button.active = playerSkillSet.attributePoints > 0;
		}
		if (name.find("increment_skill_") == 0) {
			button.active = playerSkillSet.skillPoints > 0;
		}
		window.draw(button);
	}
}

void Hud::update(float elapsedTime, const Player& player)
{
	_miniMapView.setCenter(player.playerCreature.getWorldCenter());

	std::ostringstream playerInfo;
	playerInfo.setf(std::ios::fixed);
	playerInfo << "Health: " << player.playerCreature.getHitPoints() << "/" << player.playerCreature.getMaxHitPoints() << std::endl;
	playerInfo << "Level: " << player.playerCreature.getLevel() << std::endl;
	playerInfo << "Experience: " << player.playerCreature.getExperience() << "/" << player.playerCreature.getExpRequiredToNextLevel() << std::endl;
	_text.setString(playerInfo.str());

	_logText.setString(getActionLogString());
}

std::string const* Hud::getClickedButton(int x, int y)
{
	for each (auto& pair in _skillsButtons)
	{
		auto& name = pair.first;
		auto& button = *pair.second.get();
		if (button.sprite.getGlobalBounds().contains(x, y)) {
			return (&name);
		}
	}
	return nullptr;
}

std::string Hud::getActionLogString()
{
	while (actionLog.size() > 6) {
		actionLog.pop_front();
	}
	std::string logString;

	for (auto line : actionLog) {
		logString = logString + line;
	}

	return logString;
}

Hud & Hud::operator=(Hud & other)
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

Hud& Hud::operator=(Hud&& other) noexcept
{
	_infoView = std::move(other._infoView);
	_miniMapView = std::move(other._miniMapView);
	_infoViewPort = std::move(other._infoViewPort);
	_font = std::move(other._font);
	_text = std::move(other._text);
	_text.setFont(_font);
	_logText = std::move(other._logText);
	_logText.setFont(_font);
	_skillsButtons = std::move(other._skillsButtons);
	return *this;
}

sf::Text Hud::createText(std::string text, float xPos, float yPos, int size)
{
	auto sfText = sf::Text();
	sfText.setFont(_font);
	sfText.setFillColor(sf::Color::White);
	sfText.setCharacterSize(size);
	sfText.setPosition(xPos, yPos);
	sfText.setString(text);

	return sfText;
}

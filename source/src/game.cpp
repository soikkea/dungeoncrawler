#include <SFML/Graphics.hpp>

#include "game.h"
#include "globals.h"
#include "assets.h"

Game::Game() : 
	m_player(0, 0)
{
	// Initialize RendererWindow
	m_window = new sf::RenderWindow(sf::VideoMode(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT), "Dungeoncrawler");

	_gameView = new sf::View(sf::FloatRect(0.f, 0.f, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT * 0.8f));

	m_window->setView(*_gameView);

	Assets::get().LoadTextures();
	Assets::get().LoadFonts();

	_hud = std::move(Hud(
		sf::FloatRect(0.f, 0.f, globals::SCREEN_WIDTH * 0.8f, globals::SCREEN_HEIGHT * 0.2f),
		sf::FloatRect(0.f, 0.8f, 1.f, 0.2f),
		sf::FloatRect(0.f, 0.f, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT),
		sf::FloatRect(0.8f, 0.8f, 0.2f, 0.2f)
	));

	_level = Level();

	initializeNewLevel();

	_gameStatus = GameStatus::STARTED;

	this->gameLoop();
}

Game::~Game() {
	delete m_window;
	delete _gameView;
}

void Game::initializeNewLevel()
{
	_level.initializeNewDungeon();
	auto playerPos = _level.getPlayerStartingPos();

	m_player.playerCreature.setTilePos(playerPos.x, playerPos.y);

	m_player.resetTurn();
}

void Game::gameLoop() {

	sf::Clock clock;

	float last_update_time = clock.getElapsedTime().asSeconds();

	while (m_window->isOpen()) {
		sf::Event event;
		while (m_window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				m_window->close();

			// Handle events
			switch (_gameMode)
			{
			case MODE_MENU:
				handleMenuEvent(event);
				break;
			case MODE_GAME:
				handleGameEvent(event);
				break;
			case MODE_INVENTORY:
				handleInventoryEvent(event);
				break;
			case MODE_SKILLS:
				handleSkillsEvent(event);
				break;
			default:
				break;
			}
		}

		float current_time = clock.getElapsedTime().asSeconds();
		float elapsed_time = current_time - last_update_time;
		last_update_time = current_time;
		update(elapsed_time);
		draw();
	}
}

bool Game::handleMenuEvent(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			switch (_gameStatus)
			{
			case Game::GameStatus::STARTED:
			case Game::GameStatus::PAUSED:
				_gameMode = MODE_GAME;
				_gameStatus = GameStatus::STARTED;
				return true;
			case Game::GameStatus::NOT_STARTED:
			case Game::GameStatus::PLAYER_DIED:
				m_window->close();
				return true;
			default:
				break;
			}
		default:
			break;
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			auto buttonName = _hud.getClickedButton(event.mouseButton.x, event.mouseButton.y);
			if (buttonName == nullptr)
				return true;
			else if (*buttonName == "menu_quit_game") {
				m_window->close();
				return true;
			}
			else if (*buttonName == "menu_continue_game") {
				_gameMode = MODE_GAME;
				_gameStatus = GameStatus::STARTED;
				return true;
			}
		}
	}
	return false;
}

bool Game::handleGameEvent(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code)
		{
		case sf::Keyboard::W:
			m_player.handleMoveInput(Direction::NORTH, _level);
			return true;
		case sf::Keyboard::D:
			m_player.handleMoveInput(Direction::EAST, _level);
			return true;
		case sf::Keyboard::S:
			m_player.handleMoveInput(Direction::SOUTH, _level);
			return true;
		case sf::Keyboard::A:
			m_player.handleMoveInput(Direction::WEST, _level);
			return true;
		case sf::Keyboard::I:
			_gameMode = MODE_INVENTORY;
			return true;
		case sf::Keyboard::C:
			_gameMode = MODE_SKILLS;
			return true;
		case sf::Keyboard::Escape:
			_gameStatus = GameStatus::PAUSED;
			_gameMode = MODE_MENU;
			return true;
		default:
			break;
		}
	}
	return false;
}

bool Game::handleInventoryEvent(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code)
		{
		case sf::Keyboard::Num1:
			m_player.playerCreature.useInventoryItem(0);
			return true;
		case sf::Keyboard::Num2:
			m_player.playerCreature.useInventoryItem(1);
			return true;
		case sf::Keyboard::Num3:
			m_player.playerCreature.useInventoryItem(2);
			return true;
		case sf::Keyboard::Num4:
			m_player.playerCreature.useInventoryItem(3);
			return true;
		case sf::Keyboard::Num5:
			m_player.playerCreature.useInventoryItem(4);
			return true;
		case sf::Keyboard::I:
			_gameMode = MODE_GAME;
			return true;
		default:
			break;
		}
	}
	return false;
}

bool Game::handleSkillsEvent(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code)
		{
		case sf::Keyboard::C:
			_gameMode = MODE_GAME;
			return true;
		default:
			break;
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			auto buttonName = _hud.getClickedButton(event.mouseButton.x, event.mouseButton.y);
			if (buttonName == nullptr)
				return true;
			else if (*buttonName == "increment_attr_agility") {
				m_player.playerCreature.getSkillSet().increaseAttribute(Attribute::AGILITY);
				return true;
			}
			else if (*buttonName == "increment_attr_constitution") {
				m_player.playerCreature.getSkillSet().increaseAttribute(Attribute::CONSTITUTION);
				return true;
			}
			else if (*buttonName == "increment_attr_strength") {
				m_player.playerCreature.getSkillSet().increaseAttribute(Attribute::STRENGTH);
				return true;
			}
			else if (*buttonName == "increment_skill_melee") {
				m_player.playerCreature.getSkillSet().increaseSkill(Skill::MELEE);
				return true;
			}
		}
	}
	return false;
}

void Game::draw() {
	m_window->clear();

	// Center window on player
	_gameView->setCenter(m_player.playerCreature.getWorldCenter());
	_gameView->setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 0.8f));
	m_window->setView(*_gameView);

	m_window->draw(_level);
	m_window->draw(m_player.playerCreature);

	_hud.draw(*m_window, _level);

	switch (_gameMode)
	{
	case MODE_INVENTORY:
		_hud.drawInventory(*m_window, m_player);
		break;
	case MODE_SKILLS:
		_hud.drawSkills(*m_window, m_player);
		break;
	case MODE_MENU:
		_hud.drawMenu(*m_window, m_player);
		break;
	default:
		break;
	}

	m_window->display();
}

void Game::update(float elapsedTime) {
	char buffer[100];
	snprintf(buffer, sizeof(buffer), "Dungeoncrawler, FPS=%3.2f", 1.0f/elapsedTime);
	std::string title = buffer;
	m_window->setTitle(title);

	m_player.playerCreature.calculateStats();

	if (!m_player.playerCreature.isAlive())
	{
		_gameStatus = GameStatus::PLAYER_DIED;
		_gameMode = MODE_MENU;
	}

	if (m_player.isTurnOver()) {
		_level.update(m_player);

		m_player.resetTurn();
	}

	if (_level.endReached) {
		initializeNewLevel();
		_hud.actionLog.push_back("You descended to a lower level.");
	}

	_hud.update(elapsedTime, m_player);
}
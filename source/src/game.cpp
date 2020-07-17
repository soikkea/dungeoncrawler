#include <SFML/Graphics.hpp>

#include "game.h"
#include "globals.h"
#include "assets.h"

Game::Game() : 
	m_player(0, 0)
{
	// Initialize RendererWindow
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT), "Dungeoncrawler");

	_gameView = std::make_unique<sf::View>(sf::FloatRect(0.f, 0.f, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT * 0.8f));

	_window->setView(*_gameView);

	Assets::get().LoadTextures();
	Assets::get().LoadFonts();

	_hud = std::make_unique<Hud>(
		sf::FloatRect(0.f, 0.f, globals::SCREEN_WIDTH * 0.8f, globals::SCREEN_HEIGHT * 0.2f),
		sf::FloatRect(0.f, 0.8f, 1.f, 0.2f),
		sf::FloatRect(0.f, 0.f, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT),
		sf::FloatRect(0.8f, 0.8f, 0.2f, 0.2f)
	);

	_level = Level();

	_gameStatus = GameStatus::NOT_STARTED;
	_gameMode = MODE_MENU;

	this->gameLoop();
}

Game::~Game()
{
}

bool Game::isInProgress()
{
	return _gameStatus == GameStatus::STARTED;
}

void Game::startNewGame()
{
	m_player.resetPlayer();

	initializeNewLevel();

	_gameStatus = GameStatus::STARTED;
	_gameMode = MODE_GAME;
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

	while (_window->isOpen()) {
		sf::Event event;
		while (_window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				_window->close();

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
				_gameMode = MODE_GAME;
				_gameStatus = GameStatus::STARTED;
				return true;
			case Game::GameStatus::NOT_STARTED:
			case Game::GameStatus::PLAYER_DIED:
				_window->close();
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
			auto buttonName = _hud->getClickedButton(event.mouseButton.x, event.mouseButton.y);
			if (buttonName == nullptr)
				return true;
			else if (*buttonName == "menu_new_game") {
				startNewGame();
				return true;
			}
			else if (*buttonName == "menu_quit_game") {
				_window->close();
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
			auto buttonName = _hud->getClickedButton(event.mouseButton.x, event.mouseButton.y);
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
	_window->clear();

	// Center window on player
	_gameView->setCenter(m_player.playerCreature.getWorldCenter());
	_gameView->setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 0.8f));
	_window->setView(*_gameView);


	if (_gameMode != MODE_MENU) {
		_window->draw(_level);
		_window->draw(m_player.playerCreature);

		_hud->draw(*_window, _level);
	}

	switch (_gameMode)
	{
	case MODE_INVENTORY:
		_hud->drawInventory(*_window, m_player);
		break;
	case MODE_SKILLS:
		_hud->drawSkills(*_window, m_player);
		break;
	case MODE_MENU:
		_hud->drawMenu(*_window, *this);
		break;
	default:
		break;
	}

	_window->display();
}

void Game::update(float elapsedTime) {
	char buffer[100];
	snprintf(buffer, sizeof(buffer), "Dungeoncrawler, FPS=%3.2f", 1.0f/elapsedTime);
	std::string title = buffer;
	_window->setTitle(title);

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
		_hud->actionLog.push_back("You descended to a lower level.\n");
	}

	_hud->update(elapsedTime, m_player);
}
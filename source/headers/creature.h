#pragma once

#include <map>
#include <memory>
#include <string>

#include "sprite.h"
#include "skillset.h"
#include "stat.h"

class Player;
class Item;
class Weapon;

class Creature : public Sprite
{
public:
	Creature(int x, int y);
	virtual ~Creature();
	Creature(Creature& other);
	Creature& operator=(Creature& other);

	const int getHitPoints() const;
	const int getMaxHitPoints() const;
	void gainHitpoints(int amount);
	virtual bool isAlive() const;
	virtual bool canMove() const;
	const std::string getName() const;
	void setName(std::string name);
	const int getLevel() const;
	const int getExperience() const;
	const int getExperienceWorth() const;
	const int getExpRequiredToNextLevel() const;
	void gainExperience(int amount);
	void gainLevel(int amount);
	void equipWeapon(std::unique_ptr<Weapon> weapon);
	std::vector<std::unique_ptr<Item>>& getInventory();
	bool addItem(std::unique_ptr<Item>& item);
	bool useInventoryItem(int itemIndex);
	const int getDamage() const;
	SkillSet& getSkillSet();
	void startTurn();
	virtual bool moveStep(Direction direction, const Level& level);

	void update(Level& level, Player& player);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawHealth(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void attackCreature(Creature& target);

	virtual void calculateStats(bool force = false);

	std::map<StatEnum, Stat> stats;
protected:
	std::string _name;
	int _level;
	int _experience;
	int _experienceWorth;
	int _experienceReqToNextLevel;
	std::unique_ptr<Weapon> _equippedWeapon;
	std::vector<std::unique_ptr<Item>> _inventory;
	SkillSet _skillSet;

	sf::VertexArray _sightLine;
	bool _seesPlayer;
};
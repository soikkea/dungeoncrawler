#pragma once

#include "sprite.h"

class Player;

class Creature : public Sprite
{
public:
	Creature(unsigned int x, unsigned int y);
	virtual ~Creature();

	const int getHitPoints() const;
	const int getMaxHitPoints() const;
	void gainHitpoints(int amount);
	virtual bool isAlive() const;
	const std::string getName() const;
	void setName(std::string name);
	const int getLevel() const;
	const int getExperience() const;
	const int getExperienceWorth() const;
	const int getExpRequiredToNextLevel() const;
	void gainExperience(int amount);
	void gainLevel(int amount);

	void update(Level& level, Player& player);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void attackCreature(Creature& target);
protected:
	int _maxHitPoints;
	int _hitPoints;
	std::string _name;
	int _level;
	int _experience;
	int _experienceWorth;
	int _experienceReqToNextLevel;

	sf::VertexArray _sightLine;
	bool _seesPlayer;
};
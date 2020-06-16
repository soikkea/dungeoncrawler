#pragma once

#include <map>

enum class Attribute {
	STRENGTH,
	CONSTITUTION,
	AGILITY
};

enum class Skill
{
	MELEE
};

class SkillSet {
public:
	SkillSet();
	void increaseAttribute(Attribute attribute, int value = 1);
	void increaseSkill(Skill skill, int value = 1);
	std::map<Attribute, int> attributes;
	std::map<Skill, int> skills;
};
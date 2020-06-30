#include "skillset.h"

SkillSet::SkillSet() :
	skillPoints(0),
	attributePoints(0)
{
	attributes[Attribute::AGILITY] = 1;
	attributes[Attribute::CONSTITUTION] = 1;
	attributes[Attribute::STRENGTH] = 1;
	skills[Skill::MELEE] = 1;
}

void SkillSet::increaseAttribute(Attribute attribute, int value)
{
	if (attributePoints < value)
		return;
	attributePoints -= value;
	attributes[attribute] += value;
	updated = true;
}

void SkillSet::increaseSkill(Skill skill, int value)
{
	if (skillPoints < value)
		return;
	skillPoints -= value;
	skills[skill] += value;
	updated = true;
}

#include "skillset.h"

SkillSet::SkillSet()
{
	attributes[Attribute::AGILITY] = 1;
	attributes[Attribute::CONSTITUTION] = 1;
	attributes[Attribute::STRENGTH] = 1;
	skills[Skill::MELEE] = 1;
}

void SkillSet::increaseAttribute(Attribute attribute, int value)
{
	attributes[attribute] += value;
}

void SkillSet::increaseSkill(Skill skill, int value)
{
	skills[skill] += value;
}

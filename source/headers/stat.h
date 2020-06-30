#pragma once

#include <string>
#include <utility>

enum class StatEnum {
	HitPoints,
	MovementPoints
};

struct Stat
{
	Stat() {};
	Stat(std::string name, int maxValue) :
		maxValue(maxValue),
		value(maxValue),
		name(name)
	{
	};
	static inline std::pair<StatEnum, Stat> createStatPair(StatEnum stat, std::string name, int maxValue)
	{
		return std::make_pair(stat, Stat(name, maxValue));
	}

	int addValue(int amount);
	void fill() { value = maxValue; }
	float getPercentage() const;

	int value;
	int maxValue;
	std::string name;
};
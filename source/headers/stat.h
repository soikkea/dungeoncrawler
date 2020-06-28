#pragma once

#include <string>
#include <utility>

struct Stat
{
	Stat() {};
	Stat(std::string name, int maxValue) :
		maxValue(maxValue),
		value(maxValue),
		name(name)
	{
	};
	static inline std::pair<std::string, Stat> createStatPair(std::string name, int maxValue)
	{
		return std::make_pair(name, Stat(name, maxValue));
	}

	int addValue(int amount);
	void fill() { value = maxValue; }
	float getPercentage() const;

	int value;
	int maxValue;
	std::string name;
};
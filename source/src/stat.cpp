#include "stat.h"

int Stat::addValue(int amount)
{
	value += amount;
	if (value > maxValue) {
		value = maxValue;
	}
	return value;
}

float Stat::getPercentage() const
{
	return value / (float)maxValue;
}

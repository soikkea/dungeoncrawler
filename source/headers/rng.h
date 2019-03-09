#pragma once

#include <random>

namespace random {

	const int SEED = 5;

	extern std::mt19937 generator;

	int randomInt(int i);
	int randomIntBetween(int a, int b);
}
#pragma once

#include <random>

namespace rng {

	const int SEED = 5;

	extern std::mt19937 generator;

	// Return a random int between 0 and i
	int randomInt(int i);

	// Return a random int between a and b
	int randomIntBetween(int a, int b);
}
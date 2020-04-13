#include "rng.h"

std::mt19937 rng::generator = std::mt19937(rng::SEED);

int rng::randomInt(int i)
{
	std::uniform_int_distribution<> dist(0, i);
	return dist(generator);
}

int rng::randomIntBetween(int a, int b)
{
	std::uniform_int_distribution<> dist(a, b);
	return dist(generator);
}

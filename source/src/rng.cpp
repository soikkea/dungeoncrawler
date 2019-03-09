#include "rng.h"

std::mt19937 random::generator = std::mt19937(random::SEED);

int random::randomInt(int i)
{
	std::uniform_int_distribution<> dist(0, i);
	return dist(generator);
}

int random::randomIntBetween(int a, int b)
{
	std::uniform_int_distribution<> dist(a, b);
	return dist(generator);
}

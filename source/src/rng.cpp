#include "rng.h"

std::mt19937 random::generator = std::mt19937(random::SEED);

int random::randomInt(int i)
{
	std::uniform_int_distribution<> dist(0, i);
	return dist(generator);
}

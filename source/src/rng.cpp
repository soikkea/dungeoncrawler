#include <stdexcept>
#include "rng.h"

std::mt19937 rng::generator = std::mt19937(rng::SEED);

int rng::randomInt(int i)
{
	if (i < 0) {
		throw std::invalid_argument("i can't be smaller than 0.");
	}

	std::uniform_int_distribution<> dist(0, i);
	return dist(generator);
}

int rng::randomIntBetween(int a, int b)
{
	if (b < a) {
		throw std::invalid_argument("b smaller than a.");
	}

	std::uniform_int_distribution<> dist(a, b);
	return dist(generator);
}

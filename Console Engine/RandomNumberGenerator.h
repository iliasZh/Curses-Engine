#pragma once

#include <random>
#include <cassert>
#include <vector>

class RNG
{
public:
	RNG() : uid{} {}
	int UniformIntDist(int min, int max)
	{
		assert(min < max);
		uid.param(std::uniform_int<int>::param_type{ min, max });
		return uid(rng);
	}
private:
	std::random_device					rd{};
	std::mt19937						rng{ rd() };
	std::uniform_int_distribution<int>	uid;
};
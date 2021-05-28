#pragma once

#include <random>
#include <cassert>
#include <vector>

class RNG
{
public:
	RNG() {}
	int Get(int index)
	{
		assert(!dists.empty());
		return dists[index](rng);
	}
	int AddDist(int lo, int hi)
	{
		assert(lo < hi);
		dists.emplace_back(lo, hi);
		return (int)dists.size() - 1;
	}
private:
	std::random_device rd{};
	std::mt19937 rng{ rd() };
	std::vector<std::uniform_int_distribution<int>> dists;
};
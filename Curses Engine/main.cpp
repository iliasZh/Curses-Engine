#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "WindowsFunctionality.h"
#include "Game.h"
#include "RandomNumberGenerator.h"

#include <iostream>

int main()
{
	//Game game;
	RNG rng;
	for (int i = 0; i < 100; ++i)
	{
		std::cout << rng.UniformIntDist(0, 50) << std::endl;
	}
	return 0;
}
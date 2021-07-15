#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "WindowsFunctionality.h"
#include "ConsoleWrapper.h"
#include <iostream>

int main()
{
	try
	{
		Console con{ 8u };
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	return 0;
}
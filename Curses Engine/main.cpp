#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")
#include "Console.h"
#include "Game.h"
#include "Utilities.h"

using curses::Curses;
typedef curses::Curses::Color Color;

int main()
{
	try
	{
		Console console{ 15u }; // font width = 15

		Game game{};

		while (true)
		{
			if (game.Go() == 0)
			{
				return 0;
			}
			else
			{
				continue;
			}
		}
	}
	catch (const ExceptionBase& e)
	{
		ShowErrorWindow(e);
	}
	catch (const std::exception& e)
	{
		MessageBox(NULL, to_ws(e.what()).c_str(), L"Standard C++ exception!", MB_OK | MB_ICONERROR);
	}

	return 0;
}
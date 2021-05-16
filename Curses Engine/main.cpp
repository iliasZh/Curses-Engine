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
		Game game{ 15u };

		while (true)
		{
			switch (game.Go())
			{
			case Game::Message::Ok:
				continue;
				break;
			case Game::Message::Quit:
				return 0;
				break;
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
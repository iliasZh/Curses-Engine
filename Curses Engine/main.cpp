#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "Game.h"
#include "RandomNumberGenerator.h"
#include "Menu.h"

#include <iostream>

int main()
{
	//Game game;
	Console con;
	Curses cs;
	Keyboard kbd;

	cs.SetEchoMode(false);
	cs.SetCursorMode(Curses::CursorMode::Invisible);

	EntryList el
	{
		{
			{u8"START",		{ {}				}	},
			{u8"SETTINGS",	{ {}				}	},
			{u8"ABOUT",		{ {}				}	},
			{u8"CHOOSE",	{ {u8"YES", u8"NO"}	}	},
			{u8"QUIT",		{ {}				}	}
		}
	};

	LayoutDesc ld(LayoutDesc::Pos::Centered, 5u, 2u);

	Menu menu(cs.stdwin, kbd, u8"MENU", el, ld);
	menu.Show();

	Sleep(5000);

	return 0;
}
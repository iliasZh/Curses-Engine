#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "Game.h"
#include "RandomNumberGenerator.h"
//#include "Menu.h"

#include <iostream>

int main()
{
	//Game game;
	Console con;
	Curses cs;
	cs.SetCursorMode(Curses::CursorMode::Invisible);
	cs.SetEchoMode(false);
	Curses::Window win{ 30,0,30,30 };

	cs.stdwin.Write(0, 0, u8"adgafhafh", Curses::Color::Blue, Curses::Color::Green);
	cs.stdwin.Write(0, 1, u8"adgafhafh", Curses::Color::Blue, Curses::Color::Green);
	win.Write(0, 0, u8"HELLO");
	cs.stdwin.Refresh();
	win.Refresh();
	cs.stdwin.Touch();
	cs.stdwin.Refresh();
	Sleep(2000);
	return 0;
}
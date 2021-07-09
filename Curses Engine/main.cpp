#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "Game.h"
#include "RandomNumberGenerator.h"

#include <iostream>

int main()
{
	//Game game;
	Console con;
	Curses cs;
	cs.SetCursorMode(Curses::CursorMode::Invisible);
	cs.SetEchoMode(false);

	cs.stdwin.Write(0, 0, u8"akshnfdakhfiuqh", Curses::Color::Blue, Curses::Color::Green);
	cs.stdwin.GetCh();
	return 0;
}
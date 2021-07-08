#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "Game.h"
#include "RandomNumberGenerator.h"

#include <iostream>

int main()
{
	//Game game;
	Console con;
	Curses cs;
	Curses::Window win{ 0,0,60,30 };
	win.Write(0, 0, u8"TEST");
	win.GetCh();
	return 0;
}
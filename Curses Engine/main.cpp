#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "WindowsFunctionality.h"
#include "CursesWrapper.h"

int main()
{
	Curses cs;
	Curses::Window win{ 0, 0, 30, 30 };
	win.Write(0, 0, u8"ПОЛНОСТЬЮ НА РУССКОМ ЯЗЫКЕ", Curses::Color::Blue, Curses::Color::Green);
	win.Refresh();
	Sleep(1000);
	return 0;
}
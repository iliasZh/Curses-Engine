#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "WindowsFunctionality.h"
#include "CursesWrapper.h"
#include "ConsoleWrapper.h"

int main()
{
	Console con{ 15u };
	Curses cs;
	Curses::Window win{ 0, 0, 30, 10 };
	win.Write(0, 0, u8"ПОЛНОСТЬЮ НА РУССКОМ ЯЗЫКЕ", Curses::Color::Blue, Curses::Color::Green);
	win.Refresh();
	Sleep(5000);
	return 0;
}
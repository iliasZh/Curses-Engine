#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")
#include "CursesWindow.h"
#include "Utilities.h"

using curses::Curses;

int main()
{
	try
	{
		Curses cs;
		cs.SetCursorMode(Curses::CursorMode::Invisible);
		cs.SetEchoMode(false);

		cs.AddWindow("central", (COLS - 11) / 2, (LINES - 3) / 2, 11, 3);
		cs.AddWindow("offset", (COLS - 11) / 2 - 15, (LINES - 3) / 2 + 10, 11, 3);



		cs["central"].DrawBox();
		cs["central"].Write(1, 1, u8"YOU DIED!");

		//
		//cs["offset"].DrawBox();
		//cs["offset"].Write(1, 1, u8"YOU DIED!");
		cs["central"].GetCh();
		//
		//cs.DeleteWindow("central");
		//cs["offset"].GetCh();
	}
	catch (const Curses::Exception& e)
	{
		//wchar_t consoleTitle[100];
		//GetConsoleTitle(consoleTitle, 100);
		MessageBox(NULL, e.what(), L"Curses exception!", MB_OK | MB_ICONERROR);
	}
	catch (const std::exception& e)
	{
		//wchar_t consoleTitle[100];
		//GetConsoleTitle(consoleTitle, 100);
		MessageBox(NULL, to_ws(e.what()).c_str(), L"Standard C++ exception!", MB_OK | MB_ICONERROR);
	}

	return 0;
}
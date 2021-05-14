#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")
#include "Console.h"
#include "CursesWindow.h"
#include "Utilities.h"

using curses::Curses;
typedef curses::Curses::Color Color;


int main()
{
	try
	{
		Console cnsl{60u, 30u, 15u};

		Curses cs;
		cs.SetCursorMode(Curses::CursorMode::Invisible);
		cs.SetEchoMode(false);

		cs.AddWindow("central", (COLS - 11) / 2, (LINES - 3) / 2, 11, 3);
		//cs.AddWindow("offset", (COLS - 11) / 2 - 15, (LINES - 3) / 2 + 10, 11, 3);


		cs["central"].DrawBox(Color::White, Color::Blue);
		cs["central"].Write(1, 1, u8"Y", Color::Black	, Color::Blue);
		cs["central"].Write(2, 1, u8"O", Color::Red		, Color::Blue);
		cs["central"].Write(3, 1, u8"U", Color::Green	, Color::Blue);
		cs["central"].Write(4, 1, u8" ", Color::Blue	, Color::Blue);
		cs["central"].Write(5, 1, u8"D", Color::Yellow	, Color::Blue);
		cs["central"].Write(6, 1, u8"I", Color::Cyan	, Color::Blue);
		cs["central"].Write(7, 1, u8"E", Color::Magenta	, Color::Blue);
		cs["central"].Write(8, 1, u8"D", Color::Black	, Color::Blue);
		cs["central"].Write(9, 1, u8"!", Color::Red		, Color::Blue);
		
		//cs["central"].Write(1, 1, u8"YOU DIED!");
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
	catch (const Console::Exception& e)
	{
		ShowErrorWindow(e);
	}
	catch (const std::exception& e)
	{
		//wchar_t consoleTitle[100];
		//GetConsoleTitle(consoleTitle, 100);
		MessageBox(NULL, to_ws(e.what()).c_str(), L"Standard C++ exception!", MB_OK | MB_ICONERROR);
	}

	return 0;
}
#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")
#include "CursesWindow.h"
#include "Utilities.h"

using curses::Curses;
typedef curses::Curses::Color Color;

int main()
{
	try
	{
		//--------windows-setup--------------
		SetConsoleTitle(L"Curses Engine");
		Sleep(40);
		HWND hConsole;
		if ((hConsole = FindWindow(NULL, L"Curses Engine")) == NULL)
		{
			throw std::exception("Failed to get the console handle :(");
		}

		LONG consoleStyle;

		consoleStyle = GetWindowLong(hConsole, GWL_STYLE);

		consoleStyle = consoleStyle xor WS_SIZEBOX;

		SetWindowLong(hConsole, GWL_STYLE, consoleStyle);


		unsigned width = 180u;
		unsigned height = 30u;

		SMALL_RECT r;
		r.Left = 0;
		r.Top = 0;
		r.Right = width - 1;
		r.Bottom = height - 1;
		SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &r);

		COORD c;
		c.X = width;
		c.Y = height;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), c);
		//--------windows-setup--------------


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
	catch (const std::exception& e)
	{
		//wchar_t consoleTitle[100];
		//GetConsoleTitle(consoleTitle, 100);
		MessageBox(NULL, to_ws(e.what()).c_str(), L"Standard C++ exception!", MB_OK | MB_ICONERROR);
	}

	return 0;
}
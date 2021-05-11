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

		cs.AddWindow(L"central", (COLS - 11) / 2, (LINES - 3) / 2, 11, 3);
		cs.AddWindow(L"offset", (COLS - 11) / 2 - 15, (LINES - 3) / 2 + 10, 11, 3);

		cs.GetWindow(L"central").DrawBox();
		cs.GetWindow(L"central").Write(1, 1, "YOU DIED!");

		cs.GetWindow(L"offset").DrawBox();
		cs.GetWindow(L"offset").Write(1, 1, "YOU DIED!");

		cs.GetWindow(L"wentral").GetCh();
	}
	catch (const curses::Curses::Exception& e)
	{
		wchar_t consoleTitle[100];
		GetConsoleTitle(consoleTitle, 100);
		MessageBox(NULL, e.what(), L"Curses exception!", MB_OK | MB_ICONERROR);
	}
	catch (const std::exception& e)
	{
		wchar_t consoleTitle[100];
		GetConsoleTitle(consoleTitle, 100);
		MessageBox(FindWindow(nullptr, consoleTitle), to_ws(e.what()).c_str(), L"Curses exception!", MB_OK | MB_ICONERROR);
	}

	return 0;
}
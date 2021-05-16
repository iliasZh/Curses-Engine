#include "CursesWindow.h"
#include "Utilities.h"
#include <sstream>

curses::Curses::Window::Window(int startX, int startY, int width, int height)
	: startX{ startX }
	, startY{ startY }
	, width{ width }
	, height{ height }
	, win{ newwin(height, width, startY, startX) }
{
	assert(Curses::IsInitialized());
	assert(startX >= 0 && startY >= 0);
	assert(width > 0 && height > 0);
}

curses::Curses::Window::~Window()
{
	Clear();
	delwin(win);
}

void curses::Curses::Window::DrawBox(Color fg, Color bg)
{
	const bool isDefault = (fg == Color::White) && (bg == Color::Black);
	chtype colorPair = GetColorPair(fg, bg);
	
	if (isDefault)
	{
		box(win, 0, 0);
	}
	else
	{
		wattron(win, colorPair);
		box(win, 0, 0);
		wattroff(win, colorPair);
	}

	//wrefresh(win);
}

void curses::Curses::Window::Refresh()
{
	wrefresh(win);
}

void curses::Curses::Window::Touch()
{
	touchwin(win);
}

void curses::Curses::Window::Write(int x, int y, std::u8string str, Color fg, Color bg)
{
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	assert(count_codepoints(str) < width - x + 1); // may not work properly, i don't know shit about unicode

	const bool isDefault = (fg == Color::White) && (bg == Color::Black);
	chtype colorPair = GetColorPair(fg, bg);

	if (isDefault)
	{
		mvwprintw(win, y, x, reinterpret_cast<const char*>(str.c_str()));
	}
	else
	{
		wattron(win, colorPair);
		mvwprintw(win, y, x, reinterpret_cast<const char*>(str.c_str()));
		wattroff(win, colorPair);
	}
}

void curses::Curses::Window::GetCh()
{
	flushinp();
	wgetch(win);
}

void curses::Curses::Window::Clear()
{
	wclear(win);
}

int curses::Curses::Window::GetStartX() const
{
	return startX;
}

int curses::Curses::Window::GetStartY() const
{
	return startY;
}

curses::Curses::Curses()
{
	assert(++instances == 1);

	initscr();
	SetEchoMode(echoEnabled);
	SetCursorMode(cursorMode);
	if (HasColors())
	{
		start_color();
		for (int i = 0; i < numOfColors; ++i) // foreground
		{
			for (int j = 0; j < numOfColors; ++j) // background
			{
				init_pair(i * numOfColors + j, i, j);
			}
		}
	}
	else
	{
		THROW_CURSES_EXCEPTION("Curses constructor", "your terminal does not support colors");
	}
}

curses::Curses::~Curses()
{
	--instances;
	endwin();
}

void curses::Curses::SetCursorMode(CursorMode mode)
{
	cursorMode = mode;
	curs_set((int)mode);
}

void curses::Curses::SetEchoMode(bool enable)
{
	if (enable)
		echo();
	else
		noecho();

	echoEnabled = enable;
}

chtype curses::Curses::GetColorPair(Color f, Color b)
{
	return COLOR_PAIR((int)f * numOfColors + (int)b);
}

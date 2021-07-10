#include "CursesWrapper.h"
#include "Utilities.h"
#include <sstream>

Curses::Window::Window(ucoord startX, ucoord startY, ucoord width, ucoord height)
	: startX{ startX }
	, startY{ startY }
	, width{ width }
	, height{ height }
	, win{ newwin((int)height, (int)width, (int)startY, (int)startX) }
{
	assert(Curses::IsInitialized());
	assert(width > 0u && height > 0u);
	if (startX + width > getmaxx(stdscr) || startY + height > getmaxy(stdscr))
		THROW_CURSES_EXCEPTION("Curses Window constructor", "window out of bounds!");
}

Curses::Window::~Window()
{
	Clear();
	if (win && win != stdscr) // don't delwin() nullptr's and stdscr
		delwin(win);
}

Curses::Window& Curses::Window::operator=(Window&& rhs) noexcept
{
	if (rhs.win != win)
	{
		startX = rhs.startX;
		startY = rhs.startY;
		width = rhs.width;
		height = rhs.height;

		win = rhs.win;
		rhs.win = nullptr; // now unusable, ideally rhs should be destructed immediately
	}
	return *this;
}

void Curses::Window::DrawBox(Color fg, Color bg)
{
	const bool is_default = (fg == Color::White) && (bg == Color::Black);
	chtype color_pair = GetColorPair(fg, bg);

	if (!is_default)
	{
		wattron(win, color_pair);
		box(win, 0, 0);
		wattroff(win, color_pair);
	}
	else
		box(win, 0, 0);
}

void Curses::Window::Refresh()
{
	wrefresh(win);
}

void Curses::Window::Touch()
{
	touchwin(win);
}

void Curses::Window::Write(ucoord x, ucoord y, u8str_view str, Color fg, Color bg)
{
	assert(x < width);
	assert(y < height);
	// may not work properly, i don't know shit about unicode
	assert((uint16_t)count_codepoints(str) < width - x + 1);

	const bool is_default = (fg == Color::White) && (bg == Color::Black);
	chtype color_pair = GetColorPair(fg, bg);

	if (!is_default)
	{
		wattron(win, color_pair);
		mvwprintw(win, (int)y, (int)x, str.data());
		wattroff(win, color_pair);
	}
	else
		mvwprintw(win, (int)y, (int)x, str.data());
}

void Curses::Window::GetCh()
{
	flushinp();
	wgetch(win);
}

void Curses::Window::Clear()
{
	wclear(win);
}

Curses::Curses()
{
	assert(instances == 0u);
	initscr();

	SetEchoMode(echoMode);
	SetCursorMode(cursorMode);

	// init all color pairs that we're going to use
	if (HasColors())
	{
		start_color();
		for (int i = 0; i < numOfColors; ++i) // foreground
		{
			for (int j = 0; j < numOfColors; ++j) // background
				init_pair(i * numOfColors + j, i, j);
		}
	}
	else
	{
		THROW_CURSES_EXCEPTION("Curses constructor", "your terminal does not support colors!");
	}

	++instances; // now curses are fully set up
	// std window
	stdwin = Window{0u,0u,(Window::ucoord)getmaxx(stdscr),(Window::ucoord)getmaxy(stdscr)};
}

Curses::~Curses()
{
	--instances;
	endwin();
}

void Curses::SetCursorMode(CursorMode mode)
{
	cursorMode = mode;
	curs_set((int)mode);
}

void Curses::SetEchoMode(bool enable)
{
	if (enable) echo();
	else noecho();

	echoMode = enable;
}

chtype Curses::GetColorPair(Color f, Color b)
{
	return COLOR_PAIR((int)f * (int)numOfColors + (int)b);
}
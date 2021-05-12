#include "CursesWindow.h"
#include "Utilities.h"
#include <sstream>

const wchar_t* curses::Curses::Exception::what() const noexcept
{
    std::wstringstream wss;
    wss << "Error in " << funcname << ": "
        << errorDesc << std::endl << std::endl
        << "File: " << filename << std::endl
        << "Line: " << line << std::endl;
    whatBuffer = wss.str();
    return whatBuffer.c_str();
}

curses::Curses::Curses()
{
	initscr();
	SetEchoMode(echoEnabled);
	SetCursorMode(cursorMode);
}

curses::Curses::~Curses()
{
	endwin();
}

void curses::Curses::AddWindow(std::wstring name, int startX, int startY, int width, int height)
{
	// the only method to construct in-place that seems to be working

	/*
	A container's emplace member constructs an element using the supplied arguments.

	The value_type of your map is std::pair<const int, Foo> and that type has no constructor
	taking the arguments{ 5, 5, 'a', 'b' } i.e. this wouldn't work:

	std::pair<const int, Foo> value{ 5, 5, 'a', 'b' };
	map.emplace(value);

	You need to call emplace with arguments that match one of pair's constructors.

	With a conforming C++11 implementation you can use :

	mymap.emplace(std::piecewise_construct, std::make_tuple(5), std::make_tuple(5, 'a', 'b'));
	*/

	windows.emplace
	(
		std::piecewise_construct,
		std::make_tuple(name),
		std::make_tuple(startX, startY, width, height)
	);
}

curses::Curses::Window& curses::Curses::GetWindow(std::wstring name)
{
    auto it = windows.find(name);
    if (it == windows.end())
    {
        std::wstringstream wss;
        wss << L'\"' << name << L'\"' << L" window does not exist.";
        THROW_CURSES_EXCEPTION(L"GetWindow()", wss.str());
    }
    else
    {
        return it->second;
    }
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

curses::Curses::Window::~Window()
{
	// clear box and contents
	std::string fill(width, ' ');
	for (int i = 0; i < height; ++i)
	{
		mvwprintw(win, 0, i, fill.c_str());
	}
	wrefresh(win);
	//wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	delwin(win);
}

void curses::Curses::Window::DrawBox()
{
	box(win, 0, 0);
	wrefresh(win);
}

void curses::Curses::Window::Write(int x, int y, std::u8string str)
{
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	assert(count_codepoints(str) < width - x); // may not work properly, i don't know shit about unicode
	mvwprintw(win, x, y, reinterpret_cast<const char*>(str.c_str()));
	wrefresh(win);
}

void curses::Curses::Window::GetCh()
{
	flushinp();
	wgetch(win);
}

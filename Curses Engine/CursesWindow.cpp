#include "CursesWindow.h"

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

curses::Window& curses::Curses::GetWindow(std::wstring name)
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



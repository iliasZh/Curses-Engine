#include "ConsoleWrapper.h"
#include <cassert>

Window::Buffer::Buffer(USHORT width, USHORT height)
	: width{ width }, height{ height }
	, data{ nullptr }
{
	if (width == 0u || height == 0u)
		THROW_CONSOLE_EXCEPTION("Buffer ctor", "attempting to allocate zero-size buffer");
	data = new CHAR_INFO[width * height];
	Clear(Color::Black);
}

Window::Buffer::Buffer(Buffer&& buf) noexcept
	: width{ buf.width }, height{ buf.height }
	, data{ buf.data }
{
	buf.data = nullptr;
}

Window::Buffer& Window::Buffer::operator=(Buffer&& buf) noexcept
{
	if (this != &buf)
	{
		delete[] data;
		width = buf.width;
		height = buf.height;
		data = buf.data;
		buf.data = nullptr;
	}
	return *this;
}


CHAR_INFO& Window::Buffer::At(USHORT x, USHORT y)
{
	assert(x < width&& y < height);
	return data[y * width + x];
}

void Window::Buffer::Clear(Color c)
{
	for (unsigned i = 0u; i < USHORT(width * height); ++i)
		char_info::set(data[i], L' ', c);
}


Window::Window(const Console& con, USHORT startX, USHORT startY, USHORT width, USHORT height)
	: con{ con }
	, startPos{ (SHORT)startX, (SHORT)startY }
	, buf{ width, height }
{
	assert(con.IsInitialized());
	assert(width > 1u && height > 1u);
	assert(startX + width <= con.Width());
	assert(startY + height <= con.Height());
}

Window::Window(Window&& win) noexcept
	: con{ win.con }
	, startPos{ win.startPos }
	, buf{ std::move(win.buf) }
	, bgColor{ win.bgColor }
{
	Clear();
}

Window& Window::operator=(Window&& win) noexcept
{
	if (this != &win)
	{
		assert(&con == &win.con); // console is a singleton, so it *should* be ok, but still ugh
		startPos = win.startPos;
		bgColor = win.bgColor;
		buf = std::move(win.buf);
		Clear();
	}
	return *this;
}

void Window::Write(USHORT x, USHORT y, std::wstring_view text, Color fg, Color bg)
{
	assert(x + text.size() <= Width());
	assert(y < Height());
	for (unsigned i = 0; i < text.size(); ++i)
		char_info::set(buf.At(x + i, y), text[i], fg, bg);
}

void Window::WriteChar(USHORT x, USHORT y, wchar_t ch, Color fg, Color bg)
{
	assert(x < Width() && y < Height());
	char_info::set(buf.At(x, y), ch, fg, bg);
}

void Window::DrawBox(Color fg, Color bg)
{
	WriteChar(0,				0,				L'┌', fg, bg);
	WriteChar(Width() - 1u,		0,				L'┐', fg, bg);
	WriteChar(0,				Height() - 1u,	L'└', fg, bg);
	WriteChar(Width() - 1u,		Height() - 1u,	L'┘', fg, bg);
	for (unsigned i = 1u; i < Width() - 1u; ++i)
	{
		WriteChar(i, 0, L'─', fg, bg);
		WriteChar(i, Height() - 1u, L'─', fg, bg);
	}

	for (unsigned i = 1u; i < Height() - 1u; ++i)
	{
		WriteChar(0, i, L'│', fg, bg);
		WriteChar(Width() - 1u, i, L'│', fg, bg);
	}
}

void Window::Render() const
{
	con.Render(buf.Data(), buf.Size(), startPos);
}


Console::Console(USHORT width, USHORT height, px_count fontWidth, std::wstring_view title)
	: conOut{ GetStdHandle(STD_OUTPUT_HANDLE) }
	, width{ width }, height{ height }
	, fontWidth{ fontWidth }
	, title{ title }
	, pStdwin{}
{
	SetupConsole(false);
}

Console::Console(px_count fontWidth, std::wstring_view title)
	: conOut{ GetStdHandle(STD_OUTPUT_HANDLE) }
	, width{ 0u }, height{ 0u }
	, fontWidth{ fontWidth }
	, title{ title }
	, pStdwin{}
{
	SetupConsole(true);
}

void Console::Render(const CHAR_INFO* buffer, COORD size, COORD drawStart) const
{
	assert(size.X <= width);
	assert(size.Y <= height);
	SMALL_RECT draw_region = 
	{ 
		drawStart.X, drawStart.Y, 
		drawStart.X + size.X - 1, drawStart.Y + size.Y - 1
	};
	assert(draw_region.Right < width);
	assert(draw_region.Bottom < height);

	if (WriteConsoleOutput(conOut, buffer, size, { 0,0 }, &draw_region) == 0)
		THROW_CONSOLE_EXCEPTION("Render", "failed to draw buffer");
}

void Console::SetCursorMode(Cursor mode)
{
	cursorMode = mode;
	CONSOLE_CURSOR_INFO info{};
	info.bVisible = (cursorMode != Cursor::Invisible);
	switch (cursorMode)
	{
	case Cursor::Invisible:
	case Cursor::Underline:
		info.dwSize = 1;
		break;
	case Cursor::Full:
		info.dwSize = 100;
		break;
	default:
		THROW_CONSOLE_EXCEPTION("SetCursorMode", "Invalid cursor mode");
		break;
	}

	if (SetConsoleCursorInfo(conOut, &info) == 0)
		THROW_CONSOLE_EXCEPTION("SetCursorMode", "failed to set cursor mode");
}


void Console::SetupConsole(bool maxSize)
{
	// sanity check
	assert(instances == 0u);

	if (conOut == INVALID_HANDLE_VALUE)
		THROW_CONSOLE_EXCEPTION("SetupConsole", "failed to get console handle");

	SetCursorMode(cursorMode);

	SetupFont();

	SetupScreenBuffer(maxSize);

	SetTitleAndGetHwnd();

	GetMonitorWorkAreaSize();

	SetupStyle();

	CenterWindow();

	// sanity check
	assert(++instances == 1u);

	pStdwin = std::make_unique<Window>(*this, 0u, 0u, Width(), Height());
}

void Console::SetupFont()
{
	SHORT font_w = fontWidth;
	SHORT font_h = font_w * 2;

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize = { font_w, font_h };
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");

	if (SetCurrentConsoleFontEx(conOut, FALSE, &cfi) == NULL)
		THROW_CONSOLE_EXCEPTION("Console contrustor", "failed to set the font size");

	Sleep(50);	// wait a bit for changes to apply...
}

void Console::SetupScreenBuffer(bool maxSize)
{
	auto [max_w, max_h] = GetLargestConsoleWindowSize(conOut);
	if (maxSize)
	{
		width = max_w;
		height = max_h;
	}
	else
	{
		// check that the user requested a reasonable size
		assert(width <= max_w && height <= max_h);
	}

	// at any time console window size must not exceed the size of console screen buffer
	// for that reason
	// set window size to minimum so that it is possible to set screen buffer size w/o problem
	SMALL_RECT r{ 0,0,1,1 };
	if (SetConsoleWindowInfo(conOut, TRUE, &r) == 0)
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to (temporarily) set console window size to 1*1");

	COORD c{ (SHORT)width, (SHORT)height };
	if (SetConsoleScreenBufferSize(conOut, c) == 0)
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to set requested console screen buffer size");

	r = { 0,0,SHORT(width - 1),SHORT(height - 1) };
	if (SetConsoleWindowInfo(conOut, TRUE, &r) == 0)
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to set requested console window size");
}

void Console::SetTitleAndGetHwnd()
{
	std::wstring_view setup = L"Console engine setup...";
	SetConsoleTitle(setup.data()); // set a special title to get a HWND to the console
	Sleep(50); // wait some milliseconds for title to be set...

	if ((hConsole = FindWindow(NULL, setup.data())) == NULL)
		THROW_CONSOLE_EXCEPTION("Console constructor, FindWindow()", "failed to get the console handle");

	SetConsoleTitle(title.data()); // set requested title
}

void Console::GetMonitorWorkAreaSize()
{
	assert(hConsole != NULL);

	HMONITOR h_mon = MonitorFromWindow(hConsole, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi{};
	mi.cbSize = sizeof(mi);
	if (GetMonitorInfoW(h_mon, &mi) == 0)
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to get monitor info");
	workAreaWidth = px_count(mi.rcWork.right - mi.rcWork.left);
	workAreaHeight = px_count(mi.rcWork.bottom - mi.rcWork.top);
}

void Console::SetupStyle()
{
	assert(hConsole != NULL);

	LONG console_style;
	console_style = GetWindowLong(hConsole, GWL_STYLE); // get window style

	if (console_style & WS_SIZEBOX)		// do not allow to resize window
		console_style ^= WS_SIZEBOX;		// set to false
	if (console_style & WS_MAXIMIZEBOX)	// do not allow to maximize window
		console_style ^= WS_MAXIMIZEBOX;	// set to false
	SetWindowLong(hConsole, GWL_STYLE, console_style);	// set the new style
	
	// MSDN:
	// If you have changed certain window data using SetWindowLong,
	// you must call SetWindowPos for the changes to take effect.
	// Use the following combination for uFlags:
	// SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED.
	SetWindowPos
	(
		hConsole, HWND_TOP, 0, 0, 0, 0, 
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
	);
}

void Console::CenterWindow()
{
	assert(hConsole != NULL);

	WINDOWINFO win_info;
	win_info.cbSize = sizeof(win_info);
	if (GetWindowInfo(hConsole, &win_info) == 0)
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to get window size");
	px_count width_px = px_count(win_info.rcWindow.right - win_info.rcWindow.left);
	px_count height_px = px_count(win_info.rcWindow.bottom - win_info.rcWindow.top);

	SetWindowPos
	(
		hConsole, HWND_TOP,
		((int)workAreaWidth - (int)width_px) / 2,	// may be negative! only slightly though
		((int)workAreaHeight - (int)height_px) / 2,	// may be negative! only slightly though
		width_px, height_px,
		SWP_NOSIZE									// do not change size (ignore two prev params)
	);
}



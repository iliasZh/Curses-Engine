#include "precompiled.h"
#include "ConsoleWrapper.h"

Window::Buffer::Buffer(dimensions dims_)
	: dims{ dims_ }
	, data{ nullptr }
{
	if (dims.w == 0u || dims.h == 0u)
	{
		THROW_CONSOLE_EXCEPTION("Buffer ctor", "attempting to allocate zero-size buffer");
	}
	data = new CHAR_INFO[dims.w * dims.h];
	Clear(Color::Black);
}

Window::Buffer::Buffer(Buffer&& buf) noexcept
	: dims{ buf.dims }
	, data{ buf.data }
{
	buf.data = nullptr;
}

Window::Buffer& Window::Buffer::operator=(Buffer&& buf) noexcept
{
	if (this != &buf)
	{
		delete[] data;
		dims = buf.dims;
		data = buf.data;
		buf.data = nullptr;
	}
	return *this;
}


CHAR_INFO& Window::Buffer::At(ucoord c)
{
	assert(c.x < dims.w && c.y < dims.h);
	return data[c.y * dims.w + c.x];
}

void Window::Buffer::Clear(Color c)
{
	for (USHORT i = 0u; i < dims.w * dims.h; ++i)
	{
		char_info::set(data[i], L' ', c);
	}
}


Window::Window(const Console& con, ucoord start, dimensions dims)
	: con{ con }
	, startPos{ start }
	, buf{ dims }
{
	assert(con.IsInitialized());
	assert(dims.w > 1u && dims.h > 1u);
	assert(startPos.x + dims.w <= con.Width());
	assert(startPos.y + dims.h <= con.Height());
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

void Window::Write(ucoord c, std::wstring_view text, ColorPair col)
{
	assert(c.x + text.size() <= Width());
	assert(c.y < Height());
	for (USHORT i = 0u; i < (USHORT)text.size(); ++i)
	{
		char_info::set(buf.At({ USHORT(c.x + i), c.y }), text[i], col);
	}
}

void Window::WriteChar(ucoord c, wchar_t ch, ColorPair col)
{
	assert(c.x < Width() && c.y < Height());
	char_info::set(buf.At(c), ch, col);
}

void Window::DrawBox(ColorPair col)
{
	WriteChar({ 0,				0				},	L'\x250c', col);	// '┌'
	WriteChar({ Width() - 1u,	0				},	L'\x2510', col);	// '┐'
	WriteChar({ 0,				Height() - 1u	},	L'\x2514', col);	// '└'
	WriteChar({ Width() - 1u,	Height() - 1u	},	L'\x2518', col);	// '┘'
	for (USHORT i = 1u; i < Width() - 1u; ++i)
	{
		WriteChar({ i, 0				}, L'\x2500', col);	// '─'
		WriteChar({ i, Height() - 1u	}, L'\x2500', col);	// '─'
	}

	for (USHORT i = 1u; i < Height() - 1u; ++i)
	{
		WriteChar({ 0,				i }, L'\x2502', col);	// '│'
		WriteChar({ Width() - 1u,	i }, L'\x2502', col);	// '│'
	}
}

void Window::Render() const
{
	con.Render(buf.Data(), startPos, buf.Dimensions());
}


Console::Console(dimensions dims_, px_count font_width_, std::wstring_view title_)
	: conOut{ GetStdHandle(STD_OUTPUT_HANDLE) }
	, conIn{ GetStdHandle(STD_INPUT_HANDLE) }
	, dims{ dims_ }
	, fontWidth{ font_width_ }
	, title{ title_ }
	, pStdwin{}
{
	SetupConsole(false);
}

Console::Console(px_count font_width_, std::wstring_view title_)
	: conOut{ GetStdHandle(STD_OUTPUT_HANDLE) }
	, conIn{ GetStdHandle(STD_INPUT_HANDLE) }
	, dims{ 0u, 0u }
	, fontWidth{ font_width_ }
	, title{ title_ }
	, pStdwin{}
{
	SetupConsole(true);
}

void Console::Render(const CHAR_INFO* buffer, ucoord start_pos, dimensions buf_dims) const
{
	assert(buf_dims.w <= Width());
	assert(buf_dims.h <= Height());
	SMALL_RECT draw_region = 
	{ 
		(SHORT)start_pos.x,
		(SHORT)start_pos.y, 
		(SHORT)start_pos.x + buf_dims.w - 1,
		(SHORT)start_pos.y + buf_dims.h - 1
	};
	assert(draw_region.Right	< Width());
	assert(draw_region.Bottom	< Height());

	if (WriteConsoleOutput(conOut, buffer, { (SHORT)buf_dims.w, (SHORT)buf_dims.h },
							{ 0,0 }, &draw_region) == 0)
	{
		THROW_CONSOLE_EXCEPTION("Render", "failed to draw buffer");
	}
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
	{
		THROW_CONSOLE_EXCEPTION("SetCursorMode", "failed to set cursor mode");
	}
}


void Console::SetupConsole(bool maximize_size)
{
	assert(instances == 0u); // sanity check

	if (conOut == INVALID_HANDLE_VALUE)
	{
		THROW_CONSOLE_EXCEPTION("SetupConsole", "failed to get console handle");
	}

	SetCursorMode(cursorMode);

	SetupFont();

	SetupScreenBuffer(maximize_size);

	SetTitleAndGetHwnd();

	GetMonitorWorkAreaSize();

	SetupStyle();

	CenterWindow();

	DisableTextSelection();

	assert(++instances == 1u); // sanity check

	pStdwin = std::make_unique<Window>(*this, ucoord{ 0u, 0u }, dimensions{ Width(), Height() });
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
	{
		THROW_CONSOLE_EXCEPTION("Console contrustor", "failed to set the font size");
	}

	Sleep(50);	// wait a bit for changes to apply...
}

void Console::SetupScreenBuffer(bool maximize_size)
{
	auto [max_w, max_h] = GetLargestConsoleWindowSize(conOut); // sweet c++14/17 (not sure which)
	if (maximize_size)
	{
		dims = { (USHORT)max_w, (USHORT)max_h };
	}
	else // check that the user requested a reasonable size
	{
		assert(Width() <= max_w && Height() <= max_h);
	}

	// at any time console window size must not exceed the size of console screen buffer
	// for that reason
	// set window size to minimum so that it is possible to set screen buffer size w/o problem
	SMALL_RECT r{ 0,0,1,1 };
	if (SetConsoleWindowInfo(conOut, TRUE, &r) == 0)
	{
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to (temporarily) set console window size to 1*1");
	}

	COORD c{ (SHORT)Width(), (SHORT)Height() };
	if (SetConsoleScreenBufferSize(conOut, c) == 0)
	{
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to set requested console screen buffer size");
	}

	r = { 0,0,SHORT(Width() - 1u),SHORT(Height() - 1u) };
	if (SetConsoleWindowInfo(conOut, TRUE, &r) == 0)
	{
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to set requested console window size");
	}
}

void Console::SetTitleAndGetHwnd()
{
	std::wstring_view setup = L"Console engine setup...";
	SetConsoleTitle(setup.data());	// set a special title to get a HWND to the console
	Sleep(50);						// wait for the title to be set...

	// find by title
	if ((hConsole = FindWindow(NULL, setup.data())) == NULL)
	{
		THROW_CONSOLE_EXCEPTION("Console constructor, FindWindow()", "failed to get the console handle");
	}

	SetConsoleTitle(title.data()); // set requested title
}

void Console::GetMonitorWorkAreaSize()
{
	// gets ->primary<- monitor work area size
	assert(hConsole != NULL); // sanity check

	HMONITOR h_mon = MonitorFromWindow(hConsole, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi{};
	mi.cbSize = sizeof(mi);
	if (GetMonitorInfoW(h_mon, &mi) == 0)
	{
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to get monitor info");
	}
	workAreaWidth = px_count(mi.rcWork.right - mi.rcWork.left);
	workAreaHeight = px_count(mi.rcWork.bottom - mi.rcWork.top);
}

void Console::SetupStyle()
{
	assert(hConsole != NULL); // sanity check

	LONG console_style;
	console_style = GetWindowLong(hConsole, GWL_STYLE); // get window style

	if (console_style & WS_SIZEBOX)			// do not allow to resize window
	{
		console_style ^= WS_SIZEBOX;		// set to false
	}
	if (console_style & WS_MAXIMIZEBOX)		// do not allow to maximize window
	{
		console_style ^= WS_MAXIMIZEBOX;	// set to false
	}
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

void Console::DisableTextSelection()
{
	DWORD prev_mode = 0u;

	// get mode for *input* handle
	// don't use output handle like me, stupid mistake
	// SetMode will silently return a BOOL == 0 and proceed as normal (unless you catch it like here)
	if (GetConsoleMode(conIn, &prev_mode) == 0)
	{
		THROW_CONSOLE_EXCEPTION("SetupConsole", "failed to get console mode");
	}

	// set mode with ext flags but without quick edit mode
	// this disables mouse text selection
	// profit!
	if (SetConsoleMode(conIn, ENABLE_EXTENDED_FLAGS |
		(prev_mode & ~ENABLE_QUICK_EDIT_MODE)) == 0)
	{
		THROW_CONSOLE_EXCEPTION("SetupConsole", "failed to set console mode");
	}
}

void Console::CenterWindow()
{
	assert(hConsole != NULL); // sanity check

	// get window dimensions
	WINDOWINFO win_info;
	win_info.cbSize = sizeof(win_info);
	if (GetWindowInfo(hConsole, &win_info) == 0)
	{
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to get window size");
	}
	px_count width_px	= px_count(win_info.rcWindow.right	- win_info.rcWindow.left);
	px_count height_px	= px_count(win_info.rcWindow.bottom	- win_info.rcWindow.top);

	// center it!
	SetWindowPos
	(
		hConsole, HWND_TOP,
		((int)workAreaWidth		- (int)width_px)	/ 2,	// may be negative! only slightly though
		((int)workAreaHeight	- (int)height_px)	/ 2,	// may be negative! only slightly though
		width_px, height_px,
		SWP_NOSIZE									// do not change size (ignore two prev params)
	);
}
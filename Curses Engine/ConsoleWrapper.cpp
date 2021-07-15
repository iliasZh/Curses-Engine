#include "ConsoleWrapper.h"
#include <cassert>

Console::Console(char_count width,
				char_count height, 
				scrpx_count fontWidth, 
				std::wstring title)
	: conOut{ GetStdHandle(STD_OUTPUT_HANDLE) }
	, width{ width }, height{ height }
	, fontWidth{ fontWidth }
	, title{ title }
{
	// sanity check
	assert(++instances == 1);

	SetupFont();

	SetupScreenBuffer(false);

	SetTitleAndGetHwnd();

	GetMonitorWorkAreaSize();

	SetupStyle();

	CenterWindow();
}

Console::Console(scrpx_count fontWidthPx, std::wstring title)
	: conOut{ GetStdHandle(STD_OUTPUT_HANDLE) }
	, width{ 0 }, height{ 0 }
	, fontWidth{ fontWidthPx }
	, title{ title }
{
	SetupFont();

	SetupScreenBuffer(true);

	SetTitleAndGetHwnd();

	GetMonitorWorkAreaSize();

	SetupStyle();

	CenterWindow();
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
	std::wstring setup = L"Console engine setup...";
	SetConsoleTitle(setup.c_str()); // set a special title to get a HWND to the console
	Sleep(50); // wait some milliseconds for title to be set...

	if ((hConsole = FindWindow(NULL, setup.c_str())) == NULL)
		THROW_CONSOLE_EXCEPTION("Console constructor, FindWindow()", "failed to get the console handle");

	SetConsoleTitle(title.c_str()); // set requested title
}

void Console::GetMonitorWorkAreaSize()
{
	HMONITOR h_mon = MonitorFromWindow(hConsole, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi{};
	mi.cbSize = sizeof(mi);
	if (GetMonitorInfoW(h_mon, &mi) == 0)
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to get monitor info");
	workAreaWidth = scrpx_count(mi.rcWork.right - mi.rcWork.left);
	workAreaHeight = scrpx_count(mi.rcWork.bottom - mi.rcWork.top);
}

void Console::SetupStyle()
{
	LONG console_style;
	console_style = GetWindowLong(hConsole, GWL_STYLE); // get window style

	if (console_style & WS_SIZEBOX)		// do not allow to resize window
		console_style ^= WS_SIZEBOX;		// set to false
	if (console_style & WS_MAXIMIZEBOX)	// do not allow to maximize window
		console_style ^= WS_MAXIMIZEBOX;	// set to false
	SetWindowLong(hConsole, GWL_STYLE, console_style);	// set the new style
	SetWindowPos(hConsole, HWND_TOP, 0, 0, 0, 0, 
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void Console::CenterWindow()
{
	WINDOWINFO win_info;
	win_info.cbSize = sizeof(win_info);
	if (GetWindowInfo(hConsole, &win_info) == 0)
		THROW_CONSOLE_EXCEPTION("Console ctor", "failed to get window size");
	scrpx_count width_px = scrpx_count(win_info.rcWindow.right - win_info.rcWindow.left);
	scrpx_count height_px = scrpx_count(win_info.rcWindow.bottom - win_info.rcWindow.top);

	SetWindowPos
	(
		hConsole, HWND_TOP,
		((int)workAreaWidth - (int)width_px) / 2,		// may be negative! only slightly though
		((int)workAreaHeight - (int)height_px) / 2,	// may be negative! only slightly though
		width_px, height_px,
		SWP_NOSIZE // do not change size (ignore two prev params)
	);
}

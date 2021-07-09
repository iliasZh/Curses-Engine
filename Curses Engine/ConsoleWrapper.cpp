#include "ConsoleWrapper.h"
#include <cassert>

Console::Console(scrpx_count fontWidth, std::wstring_view title)
	: consoleHandle{ GetStdHandle(STD_OUTPUT_HANDLE) }
	, fontWidth{ fontWidth }
	, title{ title }
{
	// title setup

	// sanity check
	assert(++instances == 1);
	static_assert(widthConPx <= 60u && heightConPx <= 30u);
	static_assert(widthConPx <= heightConPx * maxAspectRatio);

	std::wstring setup = L"Curses Engine Setup...";
	SetConsoleTitle(setup.c_str()); // set a special title to get a HWND to the console
	Sleep(50); // wait some milliseconds for title to be set...

	if ((hConsole = FindWindow(NULL, setup.c_str())) == NULL)
	{
		THROW_CONSOLE_EXCEPTION("Console constructor, FindWindow()", "failed to get the console handle");
	}

	SetConsoleTitle(this->title.c_str()); // set requested title
	// title setup END


	// screen dimensions lookup
	HMONITOR h_mon = MonitorFromWindow(hConsole, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi{};
	mi.cbSize = sizeof(mi);
	if (GetMonitorInfoW(h_mon, &mi) != NULL)
	{
		screenWidth = scrpx_count(mi.rcMonitor.right - mi.rcMonitor.left);
		screenHeight = scrpx_count(mi.rcMonitor.bottom - mi.rcMonitor.top);
	}
	// screen dimensions lookup END


	// window style setup
	LONG console_style;
	console_style = GetWindowLong(hConsole, GWL_STYLE); // get window style

	if (console_style & WS_SIZEBOX)		// do not allow to resize window
		console_style ^= WS_SIZEBOX;		// set to false
	if (console_style & WS_MAXIMIZEBOX)	// do not allow to maximize window
		console_style ^= WS_MAXIMIZEBOX;	// set to false

	SetWindowLong(hConsole, GWL_STYLE, console_style);	// set the new style
	// window style setup END


	// window screen buffer setup
	SMALL_RECT r;
	r.Left = 0;
	r.Top = 0;
	r.Right = width - 1;
	r.Bottom = height - 1;
	SetConsoleWindowInfo(consoleHandle, TRUE, &r);
	
	COORD c;
	c.X = width;
	c.Y = height;
	SetConsoleScreenBufferSize(consoleHandle, c);
	// window screen buffer setup END


	// font setup
	scrpx_count max_font_w = 
		std::min(scrpx_count(screenWidth  / width),
				 scrpx_count(screenHeight / (height * 2u)));
	max_font_w = std::min(scrpx_count(maxWidth / width), max_font_w);
	SHORT font_w = std::min(scrpx_count(fontWidth + 1u), max_font_w);
	// PDCurses seems to only take font height into account (?)
	// aspect ratio for a character is always w:h == 1:2 - not true actually, but almost
	SHORT font_h = font_w * 2;

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = font_w;
	cfi.dwFontSize.Y = font_h;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	if (SetCurrentConsoleFontEx(consoleHandle, FALSE, &cfi) == NULL)
	{
		THROW_CONSOLE_EXCEPTION("Console contrustor, SetCurrentConsoleFontEx()",
			"failed to set the font size");
	}
	Sleep(50);	// wait a bit for changes to apply...
	// font setup END


	// window size setup
	WINDOWINFO win_info;
	win_info.cbSize = sizeof(win_info);
	GetWindowInfo(hConsole, &win_info);
	scrpx_count widthPx		= scrpx_count(win_info.rcWindow.right  - win_info.rcWindow.left);
	scrpx_count heightPx	= scrpx_count(win_info.rcWindow.bottom - win_info.rcWindow.top);

	SetWindowPos
	(
		hConsole, HWND_TOP,
		(screenWidth - widthPx) / 2u,
		(screenHeight - heightPx) / 2u,
		widthPx, heightPx,
		SWP_NOSIZE // do not change size (ignore two prev params)
	);
	// window size setup END
}

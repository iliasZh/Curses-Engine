#include "Console.h"
#include <cassert>

Console::Console(unsigned widthConPx, unsigned heightConPx, unsigned fontWidthPx, std::wstring title)
	: consoleHandle{ GetStdHandle(STD_OUTPUT_HANDLE) }
	, widthConPx{ widthConPx }
	, heightConPx{ heightConPx }
	, fontWidthPx{ fontWidthPx }
	, title{ title }
{
	// title setup
	 
	// sanity check
	assert(float(widthConPx) / float(heightConPx) <= maxAspectRatio);
	assert(heightConPx * fontWidthPx * 2 <= maxHeightPx);

	std::wstring setup = L"Curses Engine Setup...";
	SetConsoleTitle(setup.c_str()); // set a special title to get a HWND to the console
	Sleep(40); // wait a bit for title to be set...
	
	if ((hConsole = FindWindow(NULL, setup.c_str())) == NULL)
	{
		THROW_CONSOLE_EXCEPTION("Console constructor, FindWindow()", "failed to get the console handle");
	}

	SetConsoleTitle(title.c_str()); // set requested title
	// title setup END


	// window style setup
	LONG consoleStyle;
	consoleStyle = GetWindowLong(hConsole, GWL_STYLE); // get window style

	if (consoleStyle & WS_SIZEBOX)		// do not allow to resize window
		consoleStyle ^= WS_SIZEBOX;		// set to false
	if (consoleStyle & WS_MAXIMIZEBOX)	// do not allow to maximize window
		consoleStyle ^= WS_MAXIMIZEBOX;	// set to false

	SetWindowLong(hConsole, GWL_STYLE, consoleStyle);	// set the new style
	// window style setup END


	// window screen buffer setup

	// ConPx (console pixel) is two characters on the same line
	// it is a square with a side of 2 * fontWidthPx
	unsigned width = widthConPx * 2; 
	unsigned height = heightConPx;

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
	SHORT fontW = fontWidthPx;
	// PDCurses seems to only take font height into account.
	// aspect ratio for a character is always w:h == 1:2
	SHORT fontH = fontW * 2;
	
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontW;
	cfi.dwFontSize.Y = fontH;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	if (SetCurrentConsoleFontEx(consoleHandle, FALSE, &cfi) == NULL)
	{
		THROW_CONSOLE_EXCEPTION("Console contrustor, SetCurrentConsoleFontEx()",
								"failed to set the font size");
	}
	Sleep(40);	// wait a bit for changes to apply...
	// font setup END


	// window size setup
	WINDOWINFO winInfo;
	winInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hConsole, &winInfo);
	int widthPx = winInfo.rcWindow.right - winInfo.rcWindow.left;
	int heightPx = winInfo.rcWindow.bottom - winInfo.rcWindow.top;
	
	SetWindowPos
	(
		hConsole, HWND_TOP,
		(screenWidthPx - widthPx) / 2,
		(screenHeightPx - heightPx) / 2,
		widthPx, heightPx,
		SWP_NOSIZE // do not change size (ignore two prev params)
	);
	// window size setup END
}

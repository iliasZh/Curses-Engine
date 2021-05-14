#pragma once
#include "IliasWin.h"
#include <string>

class Console
{
public:
	Console(int widthConPx = 120, int heightConPx = 30, int fontWidthPx = 15, std::wstring title = L"Curses Engine");
	~Console();
private:
	struct Config
	{
		std::wstring title{ 50, L' ' };
		LONG style = 0;
		CONSOLE_SCREEN_BUFFER_INFO screenBufInfo{};
		CONSOLE_FONT_INFOEX fontInfo{};
	};
private:
	HWND hConsole;
	HANDLE consoleHandle;
	std::wstring title;
	int widthConPx;
	int heightConPx;
	int fontWidthPx;
	Config old;
};
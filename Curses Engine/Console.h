#pragma once
#include "IliasWin.h"
#include <string>

class Console
{
public:
	Console(unsigned widthConPx = 120, unsigned heightConPx = 30, unsigned fontWidthPx = 15, std::wstring title = L"Curses Engine");
	Console(const Console&) = delete;
	Console(Console&&) = delete;
	Console& operator=(const Console&) = delete;
	Console& operator=(Console&&) = delete;
	~Console() {}
private:
	HWND hConsole;
	HANDLE consoleHandle;
	std::wstring title;
	unsigned widthConPx;
	unsigned heightConPx;
	unsigned fontWidthPx;
	unsigned screenWidthPx = 1920;
	unsigned screenHeightPx = 1080;
	float maxAspectRatio = 2.0f;
	unsigned maxHeightPx = 1000;
};
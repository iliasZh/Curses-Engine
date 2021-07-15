#pragma once

#include "ConsoleWrapper.h"
#include "CursesWrapper.h"

class Screen : public Curses::Window
{
public:
	using Curses::Window::ucoord;
	using Color = Curses::Color;
	Screen(ucoord startX, ucoord startY, ucoord widthPx, ucoord heightPx)
		: Curses::Window{ startX, startY, widthPx * 2u, heightPx }
	{
		assert(heightPx <= Console::heightConPx);
		assert(widthPx <= Console::widthConPx);
	}
	ucoord WidthPx() { return Width(); }
	ucoord HeightPx() { return Height() * 2u; }
	void PutPixel(ucoord x, ucoord y, Color c)
	{
		Write(x * 2u, y, u8"  ", c, c);
	}
	void Draw()
	{
		Refresh();
	}
};
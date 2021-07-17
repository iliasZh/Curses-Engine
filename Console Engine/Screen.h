#pragma once

#include "ConsoleWrapper.h"

class Screen : public Window
{
public:
	using ucoord = USHORT;
	using Color = Window::Color;
	Screen(const Console& con, ucoord startX, ucoord startY, ucoord widthPx, ucoord heightPx)
		: Window{ con, startX, startY, widthPx * 2u, heightPx }
	{}
	ucoord WidthPx() { return Width(); }
	ucoord HeightPx() { return Height() * 2u; }
	void PutPixel(ucoord x, ucoord y, Color c)
	{
		Write(x * 2u, y, L"  ", c, c);
	}
	void Draw()
	{
		Render();
	}
};
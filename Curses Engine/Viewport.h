#pragma once

#include "CursesWindow.h"

class Viewport : public curses::Curses::Window
{
public:
	typedef curses::Curses Curses;
	typedef Curses::Color Color;
	Viewport(int startX, int startY, int widthConPx, int heightConPx)
		: Curses::Window{ startX, startY, widthConPx * 2, heightConPx }
	{}
	int WidthConPx() const { return Width() / 2; }
	int HeightConPx() const { return Height(); }
	void PutConPixel(int x, int y, Color c);
	void ClearManual();
};
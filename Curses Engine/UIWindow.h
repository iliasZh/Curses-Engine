#pragma once

#include "CursesWindow.h"

class UIWindow : public curses::Curses::Window
{
public:
	typedef curses::Curses::Window Window;
	typedef curses::Curses::Color Color;
public:
	//UIWindow(int startX, int startY, int width, int height)
	//	: Window{ startX, startY, width, height }
	//{}
	using Window::Window;
	void WriteCentered(int line, std::u8string text, Color fg = Color::White, Color bg = Color::Black)
	{
		const int size = (int)count_codepoints(text);
		assert(size <= Width() - 2);
		Write(Width() / 2 - size / 2, line, text, fg, bg);
	}

};
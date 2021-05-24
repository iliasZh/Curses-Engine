#pragma once

#include "UIWindow.h"

class Sidebar : public UIWindow
{
public:
	using UIWindow::UIWindow;
public:
	void OnSnakeGrow() { ++score; WriteScore(); Refresh(); }
	void WriteScore()
	{ 
		ss << u8"Your score: " << score;
		WriteCentered(6, ss.str(), Color::White, Color::Blue);
		ss.str(u8"");
		ss.clear();
	}
	void WriteInfo()
	{
		WriteCentered(2, u8"Use WASD to move");
		WriteCentered(4, u8"Press Esc to quit");
	}
private:
	std::basic_stringstream<char8_t> ss;
	unsigned score = 0;
};
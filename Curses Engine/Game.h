#pragma once

#include "CursesWindow.h"

class Game
{
public:
	Game() {}
	void Go();
	void Update();
	void DrawFrame();
private:
	curses::Curses cs;
};
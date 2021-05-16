#pragma once

#include "CursesWindow.h"

class Game
{
public:
	Game();
	int Go();
	void Update();
	void DrawFrame();
private:
	curses::Curses cs;
};
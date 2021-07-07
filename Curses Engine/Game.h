#pragma once

#include "ConsoleWrapper.h"
#include "CursesWrapper.h"
#include "Keyboard.h"

class Game
{
public:
	Game()	= default;
	~Game()	= default;
	Game(Console::scrpx_count fontWidth, std::wstring title);
	Game(const Game&)				= delete;
	Game& operator=(const Game&)	= delete;

	void Go();
	void Logic();
	void DrawFrame();
private:
	Console console;
	Curses curses;
	Keyboard kbd;
};
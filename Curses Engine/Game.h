#pragma once

#include "ConsoleWrapper.h"
#include "Keyboard.h"

class Game
{
public:
	Game()	= default;
	~Game()	= default;
	Game(Console::px_count fontWidth, std::wstring_view title);
	Game(const Game&)				= delete;
	Game& operator=(const Game&)	= delete;

	void Go();
	void Logic();
	void DrawFrame();
private:
	Console console;
	Keyboard kbd;
};
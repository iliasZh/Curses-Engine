#pragma once

#include "ConsoleWrapper.h"
#include "Keyboard.h"

class Game
{
public:
			Game(Console::px_count font_width, std::wstring_view title);
			Game()					= default;
			~Game()					= default;
			Game(const Game&)		= delete;
			Game(Game&&)			= delete;
	Game&	operator=(const Game&)	= delete;
	Game&	operator=(Game&&)		= delete;

	void	Go();
	void	Logic();
	void	DrawFrame();
private:
	Console		console;
	Keyboard	kbd;
};
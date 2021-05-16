#pragma once

#include "CursesWindow.h"
#include "Console.h"

class Game
{
public:
	enum class Message
	{
		Ok,
		Quit
	};
public:
	Game(unsigned fontWidthPx = 15u, std::wstring title = L"Curses Engine");
	Message Go();
	void Update();
	void DrawFrame();
private:
	Console console;
	curses::Curses cs;
};
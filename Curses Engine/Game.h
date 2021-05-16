#pragma once

#include "CursesWindow.h"
#include "Console.h"
#include "Timer.h"

class Game
{
public:
	enum class Message
	{
		Ok,
		Quit
	};
	enum class Direction
	{
		Up, Down, Left, Right
	};
public:
	Game(unsigned fontWidthPx = 15u, std::wstring title = L"Curses Engine");
	Message Go();
	void Update();
	void DrawFrame();
private:
	Console console;
	curses::Curses cs;
private:
	Timer timer;
	int oldX = 1, oldY = 1;
	int x = 1, y = 1;
	const int minX = 1;
	const int minY = 1;
	const int maxX = Console::widthConPx - 2;
	const int maxY = Console::heightConPx - 2;
	Direction dir = Direction::Up;
	float time = 0.0f;
};
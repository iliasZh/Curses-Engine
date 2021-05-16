#pragma once

#include "CursesWindow.h"
#include "Console.h"
#include "Timer.h"

class Game
{
public:
	typedef curses::Curses Curses;
	typedef curses::Curses::Color Color;
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
	Game(const Game&)				= delete;
	Game(Game&&)					= delete;
	Game& operator=(const Game&)	= delete;
	Game& operator=(Game&&)			= delete;
	Message Go();
	void Update();
	void BeginFrame();
	void DrawFrame();
private:
	Console console;
	Curses cs;
private:
	Timer timer{};
	int x = 1, y = 1;
	const int minX = 1;
	const int minY = 1;
	const int maxX = Console::widthConPx - 2;
	const int maxY = Console::heightConPx - 2;
	Direction dir = Direction::Up;
	float time = 0.0f;
	bool posUpdated = false;
private:
	inline static int instances = 0;
};
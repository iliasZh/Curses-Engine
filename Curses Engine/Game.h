#pragma once

#include "CursesWindow.h"
#include "Console.h"
#include "Timer.h"
#include "Viewport.h"

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
	inline static int instances = 0;
private:
	//----------------USER-DEFINED-VARIABLES----------------
	Timer timer{};
	Viewport vp;
	int x = 0, y = 0;
	Direction dir = Direction::Up;
	float time = 0.0f;
	bool posUpdated = false;
	//----------------USER-DEFINED-VARIABLES----------------
};
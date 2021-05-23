#pragma once

#include "CursesWindow.h"
#include "Console.h"
#include "Timer.h"
#include "Viewport.h"
#include "Snake.h"

class Game
{
public:
	typedef curses::Curses Curses;
	typedef curses::Curses::Window Window;
	typedef curses::Curses::Color Color;
	enum class State
	{
		Ok,
		Quit
	};
public:
	Game(unsigned fontWidthPx = 15u, std::wstring title = L"Curses Engine");
	Game(const Game&)				= delete;
	Game(Game&&)					= delete;
	Game& operator=(const Game&)	= delete;
	Game& operator=(Game&&)			= delete;
	State Go();
	void Update();
	void BeginFrame();
	void DrawFrame();
private:
	Console console;
	Curses cs;
	State state;
private:
	inline static int instances = 0;
private:
	//----------------USER-DEFINED-VARIABLES----------------
	Timer timer{};
	Window vpBound;
	Viewport vp;
	Snake snake;
	float movePeriod = 0.10f;
	float time = 0.0f;
	//----------------USER-DEFINED-VARIABLES----------------
};
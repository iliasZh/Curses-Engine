#pragma once

#include "CursesWindow.h"
#include "Console.h"
#include "Timer.h"
#include "Field.h"
#include "Sidebar.h"

class Game
{
public:
	typedef curses::Curses Curses;
	typedef curses::Curses::Window Window;
	typedef curses::Curses::Color Color;
	enum class State
	{
		Menu,
		Play,
		Dead,
		Quit
	};
public:
	Game(unsigned fontWidthPx = 15u, std::wstring title = L"Curses Engine");
	Game(const Game&)				= delete;
	Game(Game&&)					= delete;
	Game& operator=(const Game&)	= delete;
	Game& operator=(Game&&)			= delete;
	bool Go();
	void Update();
	void Menu();
	void DeathMenu();
	void OnGameResume();
	void Loop();
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
	Field field;
	Field::Snake& snake;
	Window fieldBorder;
	Sidebar sidebar;
	UIWindow menu;
	UIWindow deathMenu;
	float movePeriod = 0.10f;
	float time = 0.0f;
	bool isPressed = false;
	//----------------USER-DEFINED-VARIABLES----------------
};
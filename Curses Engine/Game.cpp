#include "Game.h"

Game::Game()
	: cs{}
{
	using curses::Curses;
	cs.SetCursorMode(Curses::CursorMode::Invisible);
	cs.SetEchoMode(false);
	cs.AddWindow("msg", 50, 14, 19, 3);
	cs["msg"].DrawBox(Curses::Color::Green);
	cs["msg"].Write(1, 1, u8"Press ESC to quit", Curses::Color::Green);
}

int Game::Go()
{
	Update();
	DrawFrame();
	if (GetKeyState(VK_ESCAPE) < 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void Game::Update()
{
}

void Game::DrawFrame()
{
}
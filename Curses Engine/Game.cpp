#include "Game.h"

Game::Game(unsigned fontWidthPx, std::wstring title)
	: console{ fontWidthPx, title }	// console setup, then
	, cs {}							// curses initialization
{
	using curses::Curses;
	cs.SetCursorMode(Curses::CursorMode::Invisible);
	cs.SetEchoMode(false);
	cs.AddWindow("msg", 50, 14, 19, 3);
	cs["msg"].DrawBox(Curses::Color::Green);
	cs["msg"].Write(1, 1, u8"Press ESC to quit", Curses::Color::Green);
}

Game::Message Game::Go()
{
	Update();
	DrawFrame();
	if (GetKeyState(VK_ESCAPE) < 0)
	{
		return Game::Message::Quit;
	}
	else
	{
		return Game::Message::Ok;
	}
}

void Game::Update()
{
}

void Game::DrawFrame()
{
}
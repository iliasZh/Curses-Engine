#include "Game.h"

Game::Game(unsigned fontWidthPx, std::wstring title)
	: console{ fontWidthPx, title }	// console setup, then
	, cs{  }						// curses initialization
	, field{ 1, 1, 40 - 1, 30 - 2 }
	, snake{ field }
	, fieldBorder {0, 0, 80, 30 }
{
	assert(++instances == 1);

	cs.SetCursorMode(Curses::CursorMode::Invisible);
	cs.SetEchoMode(false);
	fieldBorder.DrawBox(Color::Red);
	fieldBorder.Refresh();
}

Game::State Game::Go()
{
	BeginFrame();
	DrawFrame();
	Update();
	if (GetKeyState(VK_ESCAPE) < 0)
	{
		state = State::Quit;
	}

	return state;
}

void Game::Update()
{
	time += timer.Mark();

	if (GetKeyState('W') < 0)
	{
		snake.OnKeyPress('W');
	}
	else if (GetKeyState('S') < 0)
	{
		snake.OnKeyPress('S');
	}
	if (GetKeyState('A') < 0)
	{
		snake.OnKeyPress('A');
	}
	else if (GetKeyState('D') < 0)
	{
		snake.OnKeyPress('D');
	}

	if (time > movePeriod)
	{
		time -= movePeriod;

		if (!snake.Move())
			state = State::Quit;
	}
}

void Game::BeginFrame()
{
	if (snake.PosUpdated())
	{
		field.Clear();
	}
}

void Game::DrawFrame()
{
	snake.Draw();
}
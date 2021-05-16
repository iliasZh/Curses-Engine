#include "Game.h"

Game::Game(unsigned fontWidthPx, std::wstring title)
	: console{ fontWidthPx, title }	// console setup, then
	, cs {}							// curses initialization
{
	assert(++instances == 1);

	cs.SetCursorMode(Curses::CursorMode::Invisible);
	cs.SetEchoMode(false);
}

Game::Message Game::Go()
{
	Update();
	BeginFrame();
	DrawFrame();
	if (GetKeyState(VK_ESCAPE) < 0)
	{
		return Message::Quit;
	}
	else
	{
		return Message::Ok;
	}

}

void Game::Update()
{
	time += timer.Mark();

	if (GetKeyState('W') < 0)
	{
		dir = Direction::Up;
	}
	else if (GetKeyState('S') < 0)
	{
		dir = Direction::Down;
	}
	if (GetKeyState('A') < 0)
	{
		dir = Direction::Left;
	}
	else if (GetKeyState('D') < 0)
	{
		dir = Direction::Right;
	}

	if (time > 0.15f)
	{
		time -= 0.15f;

		switch (dir)
		{
		case Direction::Up:
			y = std::max(y - 1, minY);
			break;
		case Direction::Down:
			y = std::min(y + 1, maxY);
			break;
		case Direction::Left:
			x = std::max(x - 1, minX);
			break;
		case Direction::Right:
			x = std::min(x + 1, maxX);
			break;
		}

		posUpdated = true;
	}
	
}

void Game::BeginFrame()
{
	if (posUpdated)
	{
		
	}
}

void Game::DrawFrame()
{
	if (posUpdated)
	{
		
		posUpdated = false;
	}
}
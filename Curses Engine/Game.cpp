#include "Game.h"

Game::Game(unsigned fontWidthPx, std::wstring title)
	: console{ fontWidthPx, title }	// console setup, then
	, cs{  }						// curses initialization
	, vp{ 1, 1, 30 - 2, 30 - 2 }
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

	if (time > 0.05f)
	{
		time -= 0.05f;

		switch (dir)
		{
		case Direction::Up:
			y = std::max(y - 1, 0);
			break;
		case Direction::Down:
			y = std::min(y + 1, vp.HeightConPx() - 1);
			break;
		case Direction::Left:
			x = std::max(x - 1, 0);
			break;
		case Direction::Right:
			x = std::min(x + 1, vp.WidthConPx() - 1);
			break;
		}

		posUpdated = true;
	}
	
}

void Game::BeginFrame()
{
	if (posUpdated)
	{
		vp.Clear();
	}
}

void Game::DrawFrame()
{
	if (posUpdated)
	{
		vp.PutConPixel(x, y, Color::Green);
		vp.Refresh();
		posUpdated = false;
	}
}
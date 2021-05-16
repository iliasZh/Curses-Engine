#include "Game.h"

Game::Game(unsigned fontWidthPx, std::wstring title)
	: console{ fontWidthPx, title }	// console setup, then
	, cs {}							// curses initialization
{
	cs.SetCursorMode(Curses::CursorMode::Invisible);
	cs.SetEchoMode(false);
	cs.AddWindow("main", 0, 0, Console::width, Console::height);
	cs["main"].DrawBox(Color::Green);
	//cs["msg"].Write(1, 1, u8"Press ESC to quit", Curses::Color::Green);
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
		cs["main"].Clear();
		cs["main"].DrawBox(Color::Green);
	}
}

void Game::DrawFrame()
{
	if (posUpdated)
	{
		cs["main"].Write(x * 2, y, u8"  ", Color::White, Color::Green);
		cs["main"].Refresh();
		posUpdated = false;
	}
}
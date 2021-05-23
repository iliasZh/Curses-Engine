#include "Game.h"

Game::Game(unsigned fontWidthPx, std::wstring title)
	: console{ fontWidthPx, title }	// console setup, then
	, cs{  }						// curses initialization
	, field{ 1, 1, 40 - 1, 30 - 2 }
	, snake{ field.snake }
	, fieldBorder{ 0, 0, 80, 30 }
	, sidebar{ 80, 0, 40, 30 }
{
	assert(++instances == 1);

	cs.SetCursorMode(Curses::CursorMode::Invisible);
	cs.SetEchoMode(false);
	fieldBorder.DrawBox(Color::Red);
	fieldBorder.Refresh();
	sidebar.DrawBox(Color::Blue);
	sidebar.Write(12, 3, u8"Use WASD to move", Color::Cyan);
	sidebar.Write(12, 6, u8"Press Esc to quit", Color::Magenta);

	ss << u8"Your score: " << score;
	sidebar.Write(12, 9, ss.str().c_str(), Color::Blue, Color::Cyan);
	ss.str(u8"");
	ss.clear();
	sidebar.Refresh();
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

		switch (snake.Move())
		{
		case Field::Snake::Event::Move:
			break;
		case Field::Snake::Event::Grow:
			++score;
			ss << u8"Your score: " << score;
			sidebar.Write(12, 9, ss.str().c_str(), Color::Blue, Color::Cyan);
			ss.str(u8"");
			ss.clear();
			sidebar.Refresh();
			break;
		case Field::Snake::Event::Collision:
			state = State::Quit;
			break;
		default:
			break;
		}
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
	field.Draw();
}
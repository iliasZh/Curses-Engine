#include "Game.h"

Game::Game(unsigned fontWidthPx, std::wstring title)
	: console{ fontWidthPx, title }	// console setup, then
	, cs{  }						// curses initialization
	, field{ 1, 1, 40 - 1, 30 - 2 }
	, snake{ field.snake }
	, fieldBorder{ 0, 0, 80, 30 }
	, sidebar{ 80, 0, 40, 30 }
	, test{ 0, 0, console.width, console.height }
{
	assert(++instances == 1);

	state = State::Menu;

	cs.SetCursorMode(Curses::CursorMode::Invisible);
	cs.SetEchoMode(false);

	test.AddButton(u8"PLAY");
	test.AddButton(u8"EXIT");
	test.Center();
	test.DrawButtons();
}

bool Game::Go()
{
	BeginFrame();
	DrawFrame();
	Update();
	if (GetKeyState(VK_ESCAPE) < 0)
	{
		state = State::Quit;
	}

	return state == State::Quit; // true == quit
}

void Game::Update()
{
	switch (state)
	{
	case State::Menu:
		if (GetKeyState(VK_UP) < 0)
		{
			if (!isPressed)
			{
				test.OnButtonPrev();
				isPressed = true;
			}
		}
		else if (GetKeyState(VK_DOWN) < 0)
		{
			if (!isPressed)
			{
				test.OnButtonNext();
				isPressed = true;
			}
		}
		else
		{
			isPressed = false;
		}

		if (GetKeyState(VK_RETURN) < 0)
		{
			switch (test.OnButtonPress())
			{
			case 0:
				state = State::Play;
				OnGameBegin();
				break;
			case 1:
				state = State::Quit;
				break;
			}
		}
		break;
	case State::Play:
		Loop();
		break;
	case State::Dead:
		break;
	default:
		break;
	}
}

void Game::OnGameBegin()
{
	fieldBorder.DrawBox(Color::Red);
	fieldBorder.Refresh();

	sidebar.DrawBox(Color::Blue);
	sidebar.WriteInfo();
	sidebar.WriteScore();
	sidebar.Refresh();

	timer.Mark();
}

void Game::Loop()
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
			sidebar.OnSnakeGrow();
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
	switch (state)
	{
	case State::Menu:
		test.Clear();
		break;
	case State::Play:
		if (snake.PosUpdated())
		{
			field.Clear();
		}
		break;
	default:
		break;
	}
	
}

void Game::DrawFrame()
{
	switch (state)
	{
	case State::Menu:
		test.DrawButtons();
		break;
	case State::Play:
		field.Draw();
	default:
		break;
	}
}
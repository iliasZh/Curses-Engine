#include "Game.h"

Game::Game(unsigned fontWidthPx, std::wstring title)
	: console{ fontWidthPx, title }	// console setup, then
	, cs{  }						// curses initialization
	, field{ 1, 1, 40 - 1, 30 - 2 }
	, snake{ field.snake }
	, fieldBorder{ 0, 0, 80, 30 }
	, sidebar{ 80, 0, 40, 30 }
	, menu{ 0, 0, console.width, console.height }
{
	assert(++instances == 1);

	state = State::Menu;

	cs.SetCursorMode(Curses::CursorMode::Invisible);
	cs.SetEchoMode(false);

	menu.AddButton(u8"PLAY");
	menu.AddButton(u8"EXIT");
	menu.Center();
	menu.DrawButtons();
}

bool Game::Go()
{
	BeginFrame();
	DrawFrame();
	Update();
	
	return state == State::Quit; // true == quit
}

void Game::Update()
{
	switch (state)
	{
	case State::Menu:
		Menu();
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

void Game::Menu()
{
	if (GetKeyState('W') < 0)
	{
		if (!isPressed)
		{
			menu.OnButtonPrev();
			isPressed = true;
		}
	}
	else if (GetKeyState('S') < 0)
	{
		if (!isPressed)
		{
			menu.OnButtonNext();
			isPressed = true;
		}
	}
	else
	{
		isPressed = false;
	}

	if (GetKeyState('F') < 0)
	{
		switch (menu.OnButtonPress())
		{
		case 0:
			state = State::Play;
			OnGameResume();
			break;
		case 1:
			state = State::Quit;
			break;
		}
	}
}

void Game::OnGameResume()
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

	if (GetKeyState(VK_ESCAPE) < 0)
	{
		menu.ChangeButton(0, u8"CONTINUE");
		state = State::Menu;
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
		menu.Clear();
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
		menu.WriteCentered(18, u8"Use W/S to choose, and F to select");
		menu.DrawButtons();
		break;
	case State::Play:
		field.Draw();
		break;
	default:
		break;
	}
}
#include "Game.h"

Game::Game(unsigned fontWidthPx, std::wstring title)
	: console{ fontWidthPx, title }	// console setup, then
	, cs{  }						// curses initialization
	, field{ 1, 1, 40 - 1, 30 - 2 }
	, snake{ field.snake }
	, fieldBorder{ 0, 0, 80, 30 }
	, sidebar{ 80, 0, 40, 30 }
	, mainMenu{ 0, 0, console.width, console.height }
	, settingsMenu{ 0, 0, console.width, console.height }
	, deathMenu{ 32, 12, 15, 7 }
	, pauseMenu{ 32, 11, 15, 8 }
{
	assert(++instances == 1);

	state = State::Menu;

	cs.SetCursorMode(Curses::CursorMode::Invisible);
	cs.SetEchoMode(false);

	deathMenu.AddButton(Buttons::TryAgain);
	deathMenu.AddButton(Buttons::MainMenu);
	deathMenu.AddButton(Buttons::Quit);
	deathMenu.Center();
	deathMenu.ShiftStartLine(1);

	pauseMenu.AddButton(Buttons::Continue);
	pauseMenu.AddButton(Buttons::Restart);
	pauseMenu.AddButton(Buttons::MainMenu);
	pauseMenu.AddButton(Buttons::Settings);
	pauseMenu.AddButton(Buttons::Quit);
	pauseMenu.Center();
	//pauseMenu.ShiftStartLine(1);

	settingsMenu.AddButton(Buttons::WrapSettingOn);
	settingsMenu.AddButton(Buttons::SnakeSpeed + Buttons::Normal);
	settingsMenu.AddButton(Buttons::Back);
	settingsMenu.Center();

	mainMenu.AddButton(Buttons::Play);
	mainMenu.AddButton(Buttons::Settings);
	mainMenu.AddButton(Buttons::Quit);
	mainMenu.Center();
	mainMenu.DrawButtons();
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
		MainMenu();
		break;
	case State::Settings:
		SettingsMenu();
		break;
	case State::Play:
		Loop();
		break;
	case State::Dead:
		DeathMenu();
		break;
	case State::Pause:
		PauseMenu();
		break;
	default:
		break;
	}
}

void Game::Reset()
{
	sidebar.ResetScore();
	field.Reset();
}

void Game::MainMenu()
{
	if (kbd.IsBindingPressedOnce(Controls::Up))
	{
		mainMenu.OnButtonPrev();
	}
	else if (kbd.IsBindingPressedOnce(Controls::Down))
	{
		mainMenu.OnButtonNext();
	}

	if (kbd.IsBindingPressedOnce(Controls::Select))
	{
		switch (mainMenu.CurrentButtonIndex())
		{
		case 0:
			OnGameResume();
			state = State::Play;
			break;
		case 1:
			isFromMainMenu = true;
			state = State::Settings;
			break;
		case 2:
			state = State::Quit;
			break;
		default:
			break;
		}
	}
}

void Game::SettingsMenu()
{
	if (kbd.IsBindingPressedOnce(Controls::Up))
	{
		settingsMenu.OnButtonPrev();
	}
	else if (kbd.IsBindingPressedOnce(Controls::Down))
	{
		settingsMenu.OnButtonNext();
	}

	if (kbd.IsBindingPressedOnce(Controls::Select))
	{
		switch (settingsMenu.CurrentButtonIndex())
		{
		case 0:
			settings.ToggleWrappingMode();
			snake.SetWrappingMode(settings.GetWrappingMode());
			if (settings.GetWrappingMode())
				settingsMenu.ChangeButton(0, Buttons::WrapSettingOn);
			else
				settingsMenu.ChangeButton(0, Buttons::WrapSettingOff);
			break;
		case 1:
			settings.ToggleSnakeSpeedFwd();
			switch (settings.GetSnakeSpeedMode())
			{
			case 0:
				settingsMenu.ChangeButton(1, Buttons::SnakeSpeed + Buttons::Slow);
				break;
			case 1:
				settingsMenu.ChangeButton(1, Buttons::SnakeSpeed + Buttons::Normal);
				break;
			case 2:
				settingsMenu.ChangeButton(1, Buttons::SnakeSpeed + Buttons::Fast);
				break;
			}
			break;
		case 2:
			settingsMenu.SetCurrButton(0);
			if (isFromMainMenu)
				state = State::Menu;
			else
			{
				OnGameResume();
				state = State::Play;
			}
			break;
		default:
			break;
		}
	}

	bool lPress = kbd.IsBindingPressedOnce(Controls::Left);
	bool rPress = kbd.IsBindingPressedOnce(Controls::Right);
	if (lPress || rPress)
	{
		switch (settingsMenu.CurrentButtonIndex())
		{
		case 0:
			settings.ToggleWrappingMode();
			if (settings.GetWrappingMode())
				settingsMenu.ChangeButton(0, Buttons::WrapSettingOn);
			else
				settingsMenu.ChangeButton(0, Buttons::WrapSettingOff);
			break;
		case 1:
			if (lPress)
				settings.ToggleSnakeSpeedBwd();
			else if (rPress)
				settings.ToggleSnakeSpeedFwd();

			switch (settings.GetSnakeSpeedMode())
			{
			case 0:
				settingsMenu.ChangeButton(1, Buttons::SnakeSpeed + Buttons::Slow);
				break;
			case 1:
				settingsMenu.ChangeButton(1, Buttons::SnakeSpeed + Buttons::Normal);
				break;
			case 2:
				settingsMenu.ChangeButton(1, Buttons::SnakeSpeed + Buttons::Fast);
				break;
			}
			break;
		default:
			break;
		}
	}
}

void Game::PauseMenu()
{
	if (kbd.IsBindingPressedOnce(Controls::Up))
	{
		pauseMenu.OnButtonPrev();
	}
	else if (kbd.IsBindingPressedOnce(Controls::Down))
	{
		pauseMenu.OnButtonNext();
	}

	if (kbd.IsBindingPressedOnce(Controls::Select))
	{
		switch (pauseMenu.CurrentButtonIndex())
		{
		case 0:
			OnGameResume();
			state = State::Play;
			break;
		case 1:
			Reset();
			OnGameResume();
			state = State::Play;
			break;
		case 2:
			Reset();
			state = State::Menu;
			break;
		case 3:
			isFromMainMenu = false;
			state = State::Settings;
			break;
		case 4:
			state = State::Quit;
			break;
		default:
			break;
		}
		pauseMenu.SetCurrButton(0);
	}
}

void Game::DeathMenu()
{
	if (kbd.IsBindingPressedOnce(Controls::Up))
	{
		deathMenu.OnButtonPrev();
	}
	else if (kbd.IsBindingPressedOnce(Controls::Down))
	{
		deathMenu.OnButtonNext();
	}

	if (kbd.IsBindingPressedOnce(Controls::Select))
	{
		switch (deathMenu.CurrentButtonIndex())
		{
		case 0:
			Reset();
			OnGameResume();
			state = State::Play;
			break;
		case 1:
			Reset();
			state = State::Menu;
			break;
		case 2:
			state = State::Quit;
			break;
		default:
			break;
		}
		deathMenu.SetCurrButton(0);
	}
}

void Game::OnGameResume()
{
	fieldBorder.DrawBox(Color::White);
	fieldBorder.Refresh();

	sidebar.DrawBox(Color::Blue);
	sidebar.WriteInfo();
	sidebar.WriteScore();
	sidebar.Refresh();

	time = 0.0f;
	timer.Mark();
}

void Game::Loop()
{
	time += timer.Mark();

	if (kbd.IsBindingPressed(Controls::Up))
	{
		snake.OnInputUp();
	}
	else if (kbd.IsBindingPressed(Controls::Down))
	{
		snake.OnInputDown();
	}

	if (kbd.IsBindingPressed(Controls::Left))
	{
		snake.OnInputLeft();
	}
	else if (kbd.IsBindingPressed(Controls::Right))
	{
		snake.OnInputRight();
	}

	if (kbd.IsBindingPressed(Controls::Back))
	{
		state = State::Pause;
	}
	
	if (time > settings.GetMovePeriod())
	{
		time -= settings.GetMovePeriod();

		switch (snake.Move())
		{
		case Field::Snake::Event::Move:
			break;
		case Field::Snake::Event::Grow:
			sidebar.OnSnakeGrow();
			break;
		case Field::Snake::Event::Collision:
			state = State::Dead;
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
		mainMenu.Clear();
		break;
	case State::Settings:
		settingsMenu.Clear();
		break;
	case State::Play:
		if (snake.PosUpdated())
		{
			field.Clear();
		}
		break;
	case State::Dead:
		deathMenu.Clear();
		break;
	case State::Pause:
		pauseMenu.Clear();
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
		mainMenu.WriteCentered(mainMenu.GetLowerLine() + 3,
			u8"Use W/S|Up/Down to navigate, and F|Enter to select");
		mainMenu.WriteCentered(mainMenu.GetUpperLine() - 3, u8"SNAKE GAME", Color::Green);
		mainMenu.DrawButtons();
		break;
	case State::Settings:
		settingsMenu.WriteCentered(settingsMenu.GetLowerLine() + 3,
			u8"Use WASD|arrows to navigate and select options");
		settingsMenu.WriteCentered(settingsMenu.GetUpperLine() - 3, u8"SETTINGS", Color::Green);
		settingsMenu.DrawButtons();
		break;
	case State::Play:
		field.Draw();
		break;
	case State::Dead:
		deathMenu.DrawBox(Color::Red);
		deathMenu.WriteCentered(deathMenu.GetUpperLine() - 2, u8"YOU DIED!", Color::Red);
		deathMenu.DrawButtons();
		break;
	case State::Pause:
		pauseMenu.DrawBox(Color::Green);
		pauseMenu.WriteCentered(pauseMenu.GetUpperLine() - 1, u8"PAUSED", Color::Yellow);
		pauseMenu.DrawButtons();
		break;
	default:
		break;
	}
}
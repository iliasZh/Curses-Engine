#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "Game.h"
#include "Menu.h"
#include "Screen.h"
#include "Timer.h"

int main()
{
	//Game game;
	Console con;
	Curses cs;
	Keyboard kbd;

	cs.SetEchoMode(false);
	cs.SetCursorMode(Curses::CursorMode::Invisible);

	Screen scr{0,0,60,30};

	struct Coord
	{
		int x;
		int y;
	};
	Coord plr{ 0,0 };
	Coord dir{ 1,0 };
	Timer t{};
	while (true)
	{
		t.Mark();
		if (kbd.IsBindingPressedOnce(Controls::Right))
		{
			dir = { 1,0 };
		}
		else if (kbd.IsBindingPressedOnce(Controls::Left))
		{
			dir = { -1,0 };
		}
		if (kbd.IsBindingPressedOnce(Controls::Up))
		{
			dir = { 0,-1 };
		}
		else if (kbd.IsBindingPressedOnce(Controls::Down))
		{
			dir = { 0,1 };
		}

		scr.PutPixel(plr.x, plr.y, Curses::Color::Black);

		plr.x += dir.x;
		plr.y += dir.y;

		scr.PutPixel(plr.x, plr.y, Curses::Color::Red);

		std::this_thread::sleep_for(std::chrono::milliseconds(int(100.0f - t.Peek())));
		scr.Draw();
	}

	return 0;
}
#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "WindowsFunctionality.h"
#include "ConsoleWrapper.h"
#include "Keyboard.h"
#include <iostream>

using namespace std::chrono_literals;

int main()
{
	try
	{
		Console con{ 8u };
		Keyboard kbd{};

		COORD size = { con.Width(), con.Height() };
		CHAR_INFO* buffer = new CHAR_INFO[size.X * size.Y];

		COORD player = { 10,5 };
		std::pair<float, float> speed;
		CHAR_INFO bg{};
		bg.Char.UnicodeChar = L' ';
		bg.Attributes = 0x0001 | 0x0010;
		CHAR_INFO pl{};
		pl.Char.UnicodeChar = L'█';
		pl.Attributes = 0x000A | 0x0000;

		for (size_t i = 0u; i < size_t(size.X) * size.Y; ++i)
			buffer[i] = bg;

		buffer[player.Y * size.X + player.X] = pl;

		while (!kbd.IsKeyPressedOnce(VK_ESCAPE))
		{
			if (kbd.IsKeyPressed('W'))
			{
				if ((speed.second -= 0.3f) < -2.0f)
					speed.second = -2.0f;
			}
			else if (kbd.IsKeyPressed('S'))
			{
				if ((speed.second += 0.3f) > +2.0f)
					speed.second = +2.0f;
			}
			else
			{
				speed.second *= 0.95f;
			}

			if (kbd.IsKeyPressed('A'))
			{
				if ((speed.first -= 0.3f) < -4.0f)
					speed.first = -4.0f;
			}
			else if (kbd.IsKeyPressed('D'))
			{
				if ((speed.first += 0.3f) > +4.0f)
					speed.first = +4.0f;
			}
			else
			{
				speed.first *= 0.95f;
			}

			buffer[player.Y * size.X + player.X] = bg;

			player.X += SHORT(speed.first);
			player.Y += SHORT(speed.second);
			if (player.X < 0)
				player.X = (size.X - 1);
			else if (player.X >= size.X)
				player.X = 0;

			if (player.Y < 0)
				player.Y = (size.Y - 1);
			else if (player.Y >= size.Y)
				player.Y = 0;

			buffer[player.Y * size.X + player.X] = pl;

			con.Draw(buffer, size, { 0,0 });

			std::this_thread::sleep_for(10ms);
		}

		delete[] buffer;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	return 0;
}
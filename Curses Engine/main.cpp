#include "WindowsFunctionality.h"
#include "ConsoleWrapper.h"
#include "Keyboard.h"
#include "CharInfo.h"
#include <iostream>

int main()
{
	try
	{
		Console con{ 10u };
		Keyboard kbd{};

		con.SetCursorMode(Console::Cursor::Invisible);

		COORD size = { (SHORT)con.Width(), (SHORT)con.Height() };
		CHAR_INFO* buffer = new CHAR_INFO[size.X * size.Y];

		struct coord
		{
			float X = 0.0f;
			float Y = 0.0f;
		};
		coord player = { 10.0f,5.0f };
		coord speed = { 0.0f, 0.0f };

		CHAR_INFO bg{};
		char_info::set(bg, L' ', char_info::Color::DarkBlue);
		
		CHAR_INFO pl{};
		char_info::set(pl, L' ', char_info::Color::Green);


		for (size_t i = 0u; i < size_t(size.X) * size.Y; ++i)
			buffer[i] = bg;

		buffer[int(player.Y) * size.X + int(player.X) * 2]		= pl;
		buffer[int(player.Y) * size.X + int(player.X) * 2 + 1]	= pl;

		while (!kbd.IsKeyPressedOnce(VK_ESCAPE))
		{
			if (kbd.IsKeyPressed('W'))
			{
				if ((speed.Y -= 0.15f) < -1.0f)
					speed.Y = -1.0f;
			}
			else if (kbd.IsKeyPressed('S'))
			{
				if ((speed.Y += 0.15f) > +1.0f)
					speed.Y = +1.0f;
			}
			else
			{
				speed.Y *= 0.98f;
			}

			if (kbd.IsKeyPressed('A'))
			{
				if ((speed.X -= 0.15f) < -1.0f)
					speed.X = -1.0f;
			}
			else if (kbd.IsKeyPressed('D'))
			{
				if ((speed.X += 0.15f) > +1.0f)
					speed.X = +1.0f;
			}
			else
			{
				speed.X *= 0.98f;
			}

			buffer[int(player.Y) * size.X + int(player.X) * 2]		= bg;
			buffer[int(player.Y) * size.X + int(player.X) * 2 + 1]	= bg;

			player.X += speed.X;
			player.Y += speed.Y;
			if (player.X < 0.0f)
			{
				player.X = 0.0f;
				speed.X = -speed.X;
			}
			else if (player.X >= (float)(size.X / 2))
			{
				player.X = float((size.X - 1) / 2);
				speed.X = -speed.X;
			}

			if (player.Y < 0.0f)
			{
				player.Y = 0.0f;
				speed.Y = -speed.Y;
			}
			else if (player.Y >= (float)size.Y)
			{
				player.Y = float(size.Y - 1);
				speed.Y = -speed.Y;
			}

			buffer[int(player.Y) * size.X + int(player.X) * 2]		= pl;
			buffer[int(player.Y) * size.X + int(player.X) * 2 + 1]	= pl;

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
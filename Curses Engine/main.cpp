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
		con.SetCursorMode(Console::Cursor::Invisible);
		Keyboard kbd{};

		Window win{ con, con.Width() / 4u, con.Height() / 4u, con.Width() / 2u, con.Height() / 2u };
		win.SetDefaultBgColor(Window::Color::Blue);
		win.Clear();

		struct coord
		{
			float X = 0.0f;
			float Y = 0.0f;
		};
		coord player = { 10.0f,5.0f };
		coord speed = { 0.0f, 0.0f };

		win.Write(USHORT(player.X) * 2, USHORT(player.Y), L"  ", Window::Color::Green, Window::Color::Green);

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

			win.Write(USHORT(player.X) * 2, USHORT(player.Y), L"  ", Window::Color::Blue, Window::Color::Blue);

			player.X += speed.X;
			player.Y += speed.Y;
			if (player.X < 0.0f)
			{
				player.X = 0.0f;
				speed.X = -speed.X;
			}
			else if (player.X >= (float)(win.Width() / 2))
			{
				player.X = float((win.Width() - 1) / 2);
				speed.X = -speed.X;
			}

			if (player.Y < 0.0f)
			{
				player.Y = 0.0f;
				speed.Y = -speed.Y;
			}
			else if (player.Y >= (float)win.Height())
			{
				player.Y = float(win.Height() - 1);
				speed.Y = -speed.Y;
			}

			win.Write(USHORT(player.X) * 2, USHORT(player.Y), L"  ", Window::Color::Green, Window::Color::Green);

			win.Render();

			std::this_thread::sleep_for(10ms);
		}

	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	return 0;
}
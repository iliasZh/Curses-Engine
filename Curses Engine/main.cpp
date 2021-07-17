#include "WindowsFunctionality.h"
#include "ConsoleWrapper.h"
#include "Keyboard.h"

#include <iostream>

int main()
{
	try
	{
		Console con{ 10u };
		con.SetCursorMode(Console::Cursor::Invisible);
		Keyboard kbd{};

		con.Stdwin().SetDefaultBgColor(Window::Color::DarkBlue);
		con.Stdwin().Clear();
		//win.DrawBox();

		struct coord
		{
			float X = 0.0f;
			float Y = 0.0f;
		};
		coord player = { (float)con.Stdwin().Width() / 4.0f, (float)con.Stdwin().Height() / 2.0f };
		coord speed = { 0.0f, 0.0f };

		con.Stdwin().Write(USHORT(player.X) * 2u, USHORT(player.Y), L"  ", Window::Color::Red, Window::Color::Red);

		while (!kbd.IsKeyPressedOnce(VK_ESCAPE))
		{
			if (kbd.IsKeyPressed('W'))
			{
				if ((speed.Y -= 0.15f) < -2.0f)
					speed.Y = -2.0f;
			}
			else if (kbd.IsKeyPressed('S'))
			{
				if ((speed.Y += 0.15f) > +2.0f)
					speed.Y = +2.0f;
			}
			else
			{
				speed.Y *= 0.98f;
			}

			if (kbd.IsKeyPressed('A'))
			{
				if ((speed.X -= 0.15f) < -2.0f)
					speed.X = -2.0f;
			}
			else if (kbd.IsKeyPressed('D'))
			{
				if ((speed.X += 0.15f) > +2.0f)
					speed.X = +2.0f;
			}
			else
			{
				speed.X *= 0.98f;
			}

			con.Stdwin().Write(USHORT(player.X) * 2u, USHORT(player.Y), L"  ", Window::Color::DarkBlue, Window::Color::DarkBlue);

			player.X += speed.X;
			player.Y += speed.Y;
			if (player.X < 0.0f)
			{
				player.X = 0.0f;
				speed.X = -speed.X;
			}
			else if (player.X >= float((con.Stdwin().Width() - 1u) / 2))
			{
				player.X = float((con.Stdwin().Width() - 1u) / 2);
				speed.X = -speed.X;
			}

			if (player.Y < 0.0f)
			{
				player.Y = 0.0f;
				speed.Y = -speed.Y;
			}
			else if (player.Y >= float(con.Stdwin().Height() - 1u))
			{
				player.Y = float(con.Stdwin().Height() - 1u);
				speed.Y = -speed.Y;
			}

			con.Stdwin().Write(USHORT(player.X) * 2u, USHORT(player.Y), L"  ", Window::Color::Red, Window::Color::Red);

			con.Stdwin().Render();

			std::this_thread::sleep_for(10ms);
		}

	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	return 0;
}
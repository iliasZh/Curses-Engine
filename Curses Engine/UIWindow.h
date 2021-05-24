#pragma once

#include "CursesWindow.h"

class UIWindow : public curses::Curses::Window
{
public:
	typedef curses::Curses::Window Window;
	typedef curses::Curses::Color Color;
public:
	using Window::Window;
	void WriteCentered(int line, std::u8string text, Color fg = Color::White, Color bg = Color::Black)
	{
		const int size = (int)count_codepoints(text);
		assert(size <= Width() - 2);
		Write(Width() / 2 - size / 2, line, text, fg, bg);
	}
	void AddButton(std::u8string text)
	{
		buttons.emplace_back(std::move(text));

		if (buttons.size() == 1) currButton = 0;
	}
	void DrawButtons()
	{
		if (!buttons.empty())
		{
			int currLine = buttonsStartLine;
			for (int i = 0; i < buttons.size(); ++i, currLine += (buttonsSpacing + 1))
			{
				std::u8string margin(buttonMargin, ' ');

				std::u8string text = margin + buttons[i] + margin;

				if (i != currButton)
					WriteCentered(currLine, text, buttonText, buttonBase);
				else
					WriteCentered(currLine, text, buttonText, buttonHighlight);
			}
		}
		Refresh();
	}
	void OnButtonNext()
	{
		if (++currButton == (int)buttons.size())
			currButton = 0;
	}
	void OnButtonPrev()
	{
		if (--currButton == -1)
			currButton = (int)buttons.size() - 1;
	}
private:
	std::vector<std::u8string> buttons;
	int buttonsStartLine = 2;				// starting line for buttons
	int buttonsSpacing = 1;					// lines in between buttons
	int currButton = -1;
	int buttonMargin = 2;
	Color buttonText = Color::Black;
	Color buttonHighlight = Color::Blue;
	Color buttonBase = Color::Cyan;
};
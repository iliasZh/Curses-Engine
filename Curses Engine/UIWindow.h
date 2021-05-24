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
			std::u8string margin(buttonMargin, ' ');
			std::u8string marginSmall(buttonMargin - 1, ' ');
			std::u8string marginL = marginSmall + u8">";
			std::u8string marginR = u8"<" + marginSmall;
			for (int i = 0; i < buttons.size(); ++i, currLine += (buttonSpacing + 1))
			{
				if (i != currButton)
				{
					std::u8string text = margin + buttons[i] + margin;
					WriteCentered(currLine, text, buttonText, buttonBase);
				}
				else
				{
					std::u8string text = marginL + buttons[i] + marginR;
					WriteCentered(currLine, text, buttonText, buttonHighlight);
				}
			}
		}
		Refresh();
	}
	void Center()
	{
		int menuHeight = (int)buttons.size() * (buttonSpacing + 1) - buttonSpacing;
		buttonsStartLine = Height() / 2 - menuHeight / 2;
	}
	void OnButtonNext()
	{
		assert(!buttons.empty());
		if (++currButton == (int)buttons.size())
			currButton = 0;
	}
	void OnButtonPrev()
	{
		assert(!buttons.empty());
		if (--currButton == -1)
			currButton = (int)buttons.size() - 1;
	}
	int OnButtonPress()
	{
		return currButton;
	}
private:
	std::vector<std::u8string> buttons;
	int buttonsStartLine = 2;				// starting line for buttons
	int buttonSpacing = 1;					// lines in between buttons
	int currButton = -1;
	int buttonMargin = 2;
	Color buttonText = Color::Black;
	Color buttonHighlight = Color::Blue;
	Color buttonBase = Color::Cyan;
};
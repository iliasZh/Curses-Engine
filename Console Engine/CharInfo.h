#pragma once

#include "WindowsFunctionality.h"

namespace char_info
{
	enum class Color
	{
		Black		= 0x00,
		DarkBlue	= 0x01,
		DarkGreen	= 0x02,
		DarkCyan	= 0x03,
		DarkRed		= 0x04,
		DarkMagenta	= 0x05,
		DarkYellow	= 0x06,
		Gray		= 0x07,
		DarkGray	= 0x08,
		Blue		= 0x09,
		Green		= 0x0A,
		Cyan		= 0x0B,
		Red			= 0x0C,
		Magenta		= 0x0D,
		Yellow		= 0x0E,
		White		= 0x0F
	};

	struct ColorPair
	{
		ColorPair(Color fg_, Color bg_) : fg{ fg_ }, bg{ bg_ } {}
		ColorPair(Color c) : fg{ c }, bg{ c } {}
		Color fg;
		Color bg;
	};

	inline void set(CHAR_INFO& ch, wchar_t symbol, ColorPair col)
	{
		ch.Char.UnicodeChar = symbol;
		ch.Attributes = (WORD)col.fg | ((WORD)col.bg << 4);
	}
	inline wchar_t get_char(const CHAR_INFO& ch)
	{
		return ch.Char.UnicodeChar;
	}
	inline ColorPair get_colors(const CHAR_INFO& ch)
	{
		return { Color(ch.Attributes & 0x0F), Color((ch.Attributes & 0xF0) >> 4) };
	}
}
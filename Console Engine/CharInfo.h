#pragma once

#include "precompiled.h"
#include "ConsoleTypes.h"

namespace char_info
{
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
#pragma once
#include <cstdint>

struct ucoord
{
	ucoord(uint16_t x_, uint16_t y_) : x{ x_ }, y{ y_ } {}
	uint16_t x;
	uint16_t y;
};
struct dimensions
{
	dimensions(uint16_t w_, uint16_t h_) : w{ w_ }, h{ h_ } {}
	uint16_t w;
	uint16_t h;
};

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
	ColorPair(Color fg_, Color bg_)	: fg{ fg_ }, bg{ bg_ } {}
	ColorPair(Color col)			: fg{ col }, bg{ col } {}
	Color fg;
	Color bg;
};
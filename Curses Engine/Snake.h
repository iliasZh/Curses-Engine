#pragma once

#include "Field.h"
#include <vector>

class Snake
{
public:
	struct Coord
	{
		int x;
		int y;
		Coord& operator+=(const Coord& c)	{ x += c.x; y += c.y; return *this; }
		Coord operator+(const Coord& c)		{ Coord tmp{ *this }; tmp += c; return tmp; }
		Coord operator*(int i)				{ return Coord{ i * x, i * y }; }
		bool operator==(const Coord& c)		{ return (x == c.x) && (y == c.y); }
		bool operator!=(const Coord& c)		{ return !(*this == c); }
	};
	//enum class Direction { Up, Down, Left, Right };
public:
	typedef curses::Curses::Color Color;
	Snake(Field& field)
		: field{ field }
		, fieldWidth{ field.WidthConPx() }
		, fieldHeight{ field.HeightConPx() }
	{
		Coord c{ -1, 0 };
		Coord head{ (fieldWidth + initialLength) / 2, fieldHeight / 2 };
		for (int i = 0; i < initialLength; ++i)
		{
			segments.emplace_back(head + c * i);
		}	
	}

	void OnKeyPress(int vkCode)
	{
		switch (vkCode)
		{
		case 'W':
			if (drawnDir != Coord{ 0, 1 })
				dir = Coord{ 0, -1 };
			break;
		case 'S':
			if (drawnDir != Coord{ 0, -1 })
				dir = Coord{ 0, 1 };
			break;
		case 'A':
			if (drawnDir != Coord{ 1, 0 })
				dir = Coord{ -1, 0 };
			break;
		case 'D':
			if (drawnDir != Coord{ -1, 0 })
				dir = Coord{ 1, 0 };
			break;
		default:
			break;
		}
	}

	bool Move()
	{
		for (int i = 3; i < segments.size() - 1; ++i)
		{
			Coord nextHeadLoc = segments[0] + dir;

			if (nextHeadLoc == segments[i] || 
				nextHeadLoc.x < 0 || !(nextHeadLoc.x < fieldWidth) ||
				nextHeadLoc.y < 0 || !(nextHeadLoc.y < fieldHeight))
			{
				return false;
			}
		}

		for (auto rit = segments.rbegin(); rit != segments.rend() - 1; ++rit)
		{
			*rit = *(rit + 1);
		}

		segments[0] += dir;
		posUpdated = true;
		return true;
	}

	void Draw()
	{
		if (posUpdated)
		{
			field.PutConPixel(segments[0].x, segments[0].y, headColor);

			for (auto it = segments.begin() + 1; it != segments.end(); ++it)
			{
				field.PutConPixel(it->x, it->y, bodyColor);
			}
			field.Refresh();

			drawnDir = dir;

			posUpdated = false;
		}
	}

	bool PosUpdated() const { return posUpdated; }
private:
	Field& field;
	Coord drawnDir{};
	Coord dir{ 1, 0 };
	int fieldWidth;
	int fieldHeight;
	const int initialLength = 5;
	const Color headColor = Color::Yellow;
	const Color bodyColor = Color::Green;
	bool posUpdated = true;
	std::vector<Coord> segments;
};
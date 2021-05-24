#pragma once

#include "CursesWindow.h"
#include <stdlib.h>
#include <time.h>
#include <vector>

class Field : public curses::Curses::Window
{
public:
	typedef curses::Curses Curses;
	typedef Curses::Color Color;
	struct Coord
	{
		int x;
		int y;
	public:
		Coord& operator+=(const Coord& c) { x += c.x; y += c.y; return *this; }
		Coord operator+(const Coord& c) { Coord tmp{ *this }; tmp += c; return tmp; }
		Coord operator*(int i) { return Coord{ i * x, i * y }; }
		bool operator==(const Coord& c) { return (x == c.x) && (y == c.y); }
		bool operator!=(const Coord& c) { return !(*this == c); }
	};
public:
	class Fruit
	{
	public:
		Fruit(Field& field, const std::vector<Coord>& snake, const std::vector<Fruit>& fruits);
		Fruit(const Fruit& f) : pos{ f.pos }, field{ f.field } {}
		Fruit& operator=(const Fruit& f) { pos = f.pos; return *this; }
		void Draw() const;
		const Coord& Pos() const { return pos; }
	private:
		bool IsColliding(Coord c, const std::vector<Coord>& snake, const std::vector<Fruit>& fruits);
	private:
		Coord pos{ 0,0 };
		Field& field;
		const Color c = Color::Red;
	};

	class Snake
	{
	public:
		enum class Event
		{
			Move, Grow, Collision
		};
	public:
		Snake(Field& field, std::vector<Fruit>& fruits);

		void OnKeyPress(int vkCode);
		Event Move();
		void Draw();
		void Pop();
		
		void Reset();

		bool PosUpdated() const { return posUpdated; }
		const std::vector<Coord>& GetBody() const { return segments; }
	public:
		const int initialLength = 5;
	private:
		Field& field;
		std::vector<Fruit>& fruits;
		Coord drawnDir{};
		Coord dir{ 1,0 };
		int fieldWidth, fieldHeight;
		const Color headColor = Color::Yellow;
		const Color bodyColor = Color::Green;
		bool posUpdated = true;
		std::vector<Coord> segments;
	};
	

public:
	Field(int startX, int startY, int widthConPx, int heightConPx);
	Field(const Field&) = delete;
	Field(Field&&) = delete;
	Field& operator=(const Field&) = delete;
	Field& operator=(Field&&) = delete;

	void Reset();

	int WidthConPx() const { return Width() / 2; }
	int HeightConPx() const { return Height(); }
	void PutConPixel(int x, int y, Color c);
	void PutConPixel(Coord pos, Color c) { PutConPixel(pos.x, pos.y, c); }

	void Draw()
	{
		snake.Draw();
		for (const auto& f : fruits)
		{
			f.Draw();
		}
	}
public:
	Snake snake;
	std::vector<Fruit> fruits{};
	const int nFruits = 3;
};
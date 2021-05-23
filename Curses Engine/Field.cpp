#include "Field.h"

Field::Field(int startX, int startY, int widthConPx, int heightConPx)
	: Curses::Window{ startX, startY, widthConPx * 2, heightConPx }
	, snake{ *this }
{
	for (int i = 0; i < nFruits; ++i)
	{
		fruits.emplace_back(*this, snake.GetBody(), fruits);
	}
}

void Field::PutConPixel(int x, int y, Color c)
{
	assert(x >= 0 && x < WidthConPx());
	assert(y >= 0 && y < HeightConPx());
	Write(2 * x, y, u8"  ", Color::White, c);
}



Field::Snake::Snake(Field& field)
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

void Field::Snake::OnKeyPress(int vkCode)
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

bool Field::Snake::Move()
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

void Field::Snake::Draw()
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



Field::Fruit::Fruit(Field& field, const std::vector<Coord>& snake, const std::vector<Fruit>& fruits)
	: field{ field }
{
	Coord candidate;
	srand((unsigned)time(NULL));
	do
	{
		candidate = { rand() % field.WidthConPx(), rand() % field.HeightConPx() };
	}
	while (IsColliding(candidate, snake, fruits));

	pos = candidate;
}

void Field::Fruit::Draw() const
{
	field.PutConPixel(pos, c);
	field.Refresh();
}

bool Field::Fruit::IsColliding(Coord c, const std::vector<Coord>& snake, const std::vector<Fruit>& fruits)
{
	for (const auto& segment : snake)
	{
		if (c == segment)
			return true;
	}
	for (const auto& fruit : fruits)
	{
		if (c == fruit.pos)
			return true;
	}
	return false;
}

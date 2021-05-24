#include "Field.h"

Field::Field(int startX, int startY, int widthConPx, int heightConPx)
	: Curses::Window{ startX, startY, widthConPx * 2, heightConPx }
	, snake{ *this, fruits }
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

void Field::Reset()
{
	while (!fruits.empty())
	{
		fruits.pop_back();
	}
	assert(fruits.empty());

	snake.Reset();

	for (int i = 0; i < nFruits; ++i)
	{
		fruits.emplace_back(*this, snake.GetBody(), fruits);
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
	} 	while (IsColliding(candidate, snake, fruits));

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




Field::Snake::Snake(Field& field, std::vector<Fruit>& fruits)
	: field{ field }
	, fruits{ fruits }
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

Field::Snake::Event Field::Snake::Move()
{
	// check for self-collisions and wall collisions
	Coord nextHeadLoc = segments[0] + dir;
	for (int i = 3; i < segments.size() - 1; ++i)
	{
		if (nextHeadLoc == segments[i] ||
			nextHeadLoc.x < 0 || !(nextHeadLoc.x < fieldWidth) ||
			nextHeadLoc.y < 0 || !(nextHeadLoc.y < fieldHeight))
		{
			return Event::Collision;
		}
	}

	// check for collisions with fruit
	bool grow = false;
	for (auto it = fruits.begin(); it != fruits.end(); ++it)
	{
		if (nextHeadLoc == it->Pos())
		{
			grow = true;
			std::swap(*it, fruits.back());
			fruits.pop_back();
			fruits.emplace_back(field, GetBody(), fruits);
			break;
		}
	}


	// move
	Coord last = segments[segments.size() - 1];
	for (auto rit = segments.rbegin(); rit != segments.rend() - 1; ++rit)
	{
		*rit = *(rit + 1);
	}
	segments[0] += dir;

	// grow if needed
	if (grow) segments.emplace_back(last);

	posUpdated = true;
	
	if (grow)
		return Event::Grow;
	else
		return Event::Move;
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

void Field::Snake::Pop()
{
	segments.pop_back();
}

void Field::Snake::Reset()
{
	for (int i = (int)segments.size(); i > initialLength; --i)
	{
		segments.pop_back();
	}

	assert(segments.size() == initialLength);

	Coord c{ -1, 0 };
	Coord head{ (fieldWidth + initialLength) / 2, fieldHeight / 2 };
	for (int i = 0; i < initialLength; ++i)
	{
		segments[i] = head + c * i;
	}

	dir = { 1,0 };
	drawnDir = { 1,0 };

	posUpdated = true;
}



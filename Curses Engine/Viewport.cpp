#include "Viewport.h"

void Viewport::PutConPixel(int x, int y, Color c)
{
	assert(x >= 0 && x < WidthConPx());
	assert(y >= 0 && y < HeightConPx());
	Write(2 * x, y, u8"  ", Color::White, c);
}

void Viewport::ClearManual()
{
	for (int i = 0; i < Height(); ++i)
	{
		Write(0, i, std::u8string(Width(), u8' '));
	}
}

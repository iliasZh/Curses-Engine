#pragma once

#include "ConsoleWrapper.h"

class Screen : public Window
{
public:
	Screen(const Console& con, ucoord start_pos, dimensions dims)
		: Window{ con, start_pos, { dims.w * 2u, dims.h } }
	{}
	void PutPixel(ucoord c, Color col)
	{
		c.x *= 2u;
		Write(c, L"  ", { col });
	}

	void		Draw()		const { Render(); }
	uint16_t	WidthPx()	const { return Width() / 2u; }
	uint16_t	HeightPx()	const { return Height(); }
};
#pragma once

#include <bitset>

class Mouse
{
public:
			Mouse(HWND);
	bool	IsLeftPressed();
	bool	IsMiddlePressed();
	bool	IsLeftPressedOnce();
	bool	IsMiddlePressedOnce();
	POINT	GetPos();
private:
	HWND			hwnd;
	std::bitset<2>	wasPressed;
};
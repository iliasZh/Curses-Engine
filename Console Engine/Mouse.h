#pragma once

#include "WindowsFunctionality.h"
#include <bitset>

class Mouse
{
public:
	Mouse(HWND hwnd);
	bool IsLeftPressed();
	bool IsMiddlePressed();
	bool IsLeftPressedOnce();
	bool IsMiddlePressedOnce();
	POINT GetPos();
private:
	HWND hwnd;
	std::bitset<2> wasPressed;
};
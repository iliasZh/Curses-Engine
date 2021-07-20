#include "Mouse.h"

Mouse::Mouse(HWND hwnd)
	: hwnd{ hwnd }
{
	wasPressed.reset();
}

bool Mouse::IsLeftPressed()
{
	return (GetKeyState(VK_LBUTTON) & 0b1000'0000);
}

bool Mouse::IsMiddlePressed()
{
	return (GetKeyState(VK_MBUTTON) & 0b1000'0000);
}

bool Mouse::IsLeftPressedOnce()
{
	if (IsLeftPressed())
	{
		if (wasPressed[0]) return false;
		wasPressed[0] = true;
	}
	else
	{
		wasPressed[0] = false;
	}
	return wasPressed[0];
}

bool Mouse::IsMiddlePressedOnce()
{
	if (IsMiddlePressed())
	{
		if (wasPressed[1]) return false;
		wasPressed[1] = true;
	}
	else
	{
		wasPressed[1] = false;
	}
	return wasPressed[1];
}

POINT Mouse::GetPos()
{
	POINT p{};
	GetCursorPos(&p);
	ScreenToClient(hwnd, &p);
	return p;
}

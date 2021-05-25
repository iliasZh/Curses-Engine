#pragma once

#include "IliasWin.h"

class Keyboard
{
public:
	bool IsKeyPressed(int vkcode) { return GetKeyState(vkcode) < 0; }
	bool IsKeyPressedOnce(int vkcode)
	{
		if (GetKeyState(vkcode) < 0)
		{
			if (!isPressed)
			{
				isPressed = true;
				return true;
			}
			else
				return false;
		}
		else
		{
			isPressed = false;
			return false;
		}
	}
private:
	bool isPressed = false;
};
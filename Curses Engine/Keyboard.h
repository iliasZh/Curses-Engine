#pragma once

#include "Controls.h"
#include <bitset>

class Keyboard
{
public:
	bool IsKeyPressed(int vkcode) { return GetKeyState(vkcode) < 0; }
	bool IsKeyPressedOnce(int vkcode)
	{
		if (IsKeyPressed(vkcode))
		{
			if (!pressedOnce[vkcode])
			{
				pressedOnce[vkcode] = true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			pressedOnce[vkcode] = false;
		}
		return pressedOnce[vkcode];
	}

	bool IsBindingPressed(Controls::Binding b)
	{
		return IsKeyPressed(b.Primary()) || IsKeyPressed(b.Secondary());
	}
	bool IsBindingPressedOnce(Controls::Binding b)
	{
		return IsKeyPressedOnce(b.Primary()) || IsKeyPressedOnce(b.Secondary());
	}
private:
	static constexpr unsigned nKeys = 256u;
	std::bitset<nKeys> pressedOnce{ 0 };
};
#include "precompiled.h"
#include "Keyboard.h"

Keyboard::Keyboard()
{
	was_pressed.reset();
}

bool Keyboard::IsKeyPressed(int vk_code)
{
	return (GetKeyState(vk_code) & 0b1000'0000);
}

bool Keyboard::IsKeyPressedOnce(int vk_code)
{
	if (IsKeyPressed(vk_code))
	{
		if (was_pressed[vk_code]) return false;
		was_pressed[vk_code] = true;
	}
	else
	{
		was_pressed[vk_code] = false;
	}
	return was_pressed[vk_code];
}

bool Keyboard::IsBindingPressed(Controls::Binding b)
{
	return IsKeyPressed(b.Primary()) || IsKeyPressed(b.Secondary());
}

bool Keyboard::IsBindingPressedOnce(Controls::Binding b)
{
	return IsKeyPressedOnce(b.Primary()) || IsKeyPressedOnce(b.Secondary());
}
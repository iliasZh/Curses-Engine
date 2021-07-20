#include "Keyboard.h"
#include "WindowsFunctionality.h"

Keyboard::Keyboard()
{
	was_pressed.reset();
}

bool Keyboard::IsKeyPressed(int vk_code)
{
	return (GetKeyState(vk_code) & 0b1000'0000);
}

bool Keyboard::IsKeyPressedOnce(int vkcode)
{
	if (IsKeyPressed(vkcode))
	{
		if (was_pressed[vkcode]) return false;
		was_pressed[vkcode] = true;
	}
	else
	{
		was_pressed[vkcode] = false;
	}
	return was_pressed[vkcode];
}

bool Keyboard::IsBindingPressed(Controls::Binding b)
{
	return IsKeyPressed(b.Primary()) || IsKeyPressed(b.Secondary());
}

bool Keyboard::IsBindingPressedOnce(Controls::Binding b)
{
	return IsKeyPressedOnce(b.Primary()) || IsKeyPressedOnce(b.Secondary());
}
#pragma once

#include "Controls.h"
#include <bitset>

class Keyboard
{
public:
	Keyboard();
	bool IsKeyPressed(int vk_code);
	bool IsKeyPressedOnce(int vkcode);

	bool IsBindingPressed(Controls::Binding b);
	bool IsBindingPressedOnce(Controls::Binding b);
private:
	static constexpr size_t num_keys = 256u;
	std::bitset<num_keys> was_pressed;
};
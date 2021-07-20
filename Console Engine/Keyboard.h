#pragma once

#include "Controls.h"
#include <bitset>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

using namespace std::chrono_literals;

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
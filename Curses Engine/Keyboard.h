#pragma once

#include "Controls.h"
#include <bitset>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

class Keyboard
{
public:
	Keyboard()
	{
		// prevent accidentally reading bullshit values before the first keyboard poll runs
		keystates.reset();

		poll_thread = std::thread(&Keyboard::PollKeyboard, this);
	}
	~Keyboard()
	{
		end_polling.store(true);
		poll_thread.join();
	}
	bool IsKeyPressed(int vk_code) { return ReadWriteKeystates(true, vk_code); }
	bool IsKeyPressedOnce(int vkcode)
	{
		if (IsKeyPressed(vkcode))
		{
			if (was_pressed[vkcode]) return false;
			was_pressed[vkcode] = true;
		}
		else
			was_pressed[vkcode] = false;
		return was_pressed[vkcode];
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
	void PollKeyboard()
	{
		while (!end_polling.load())
		{
			ReadWriteKeystates(false);
			std::this_thread::sleep_for(polling_period);
		}
	}
	bool ReadWriteKeystates(bool is_read, int vk_code = 0)
	{
		std::lock_guard lg{ m }; // ensure no two threads can read/write at the same time
		if (is_read)
			return keystates[vk_code];
		else
		{
			for (int i = 0u; i < num_keys; ++i)
				keystates[i] = bool(GetKeyState(i) & 0b1000'0000); // high-order bit set == pressed
			return true;
		}
	}
private:
	static constexpr size_t num_keys = 256u;
	std::bitset<num_keys> keystates;
	std::bitset<num_keys> was_pressed;
	std::thread poll_thread;
	std::mutex m;
	std::chrono::milliseconds polling_period = std::chrono::milliseconds(15u);
	std::atomic_bool end_polling = false;
};
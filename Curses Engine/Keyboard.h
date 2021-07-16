#pragma once

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
	~Keyboard();
	bool IsKeyPressed(int vk_code);
	bool IsKeyPressedOnce(int vkcode);
private:
	void PollKeyboard();
	bool ReadWriteKeystates(bool is_read, int vk_code = 0);
private:
	static constexpr size_t num_keys = 256u;
	std::bitset<num_keys> keystates;
	std::bitset<num_keys> was_pressed;
	std::thread poll_thread;
	mutable std::mutex m;
	std::chrono::milliseconds polling_period = 15ms;
	std::atomic_bool end_polling = false;
};
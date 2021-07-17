#include "Keyboard.h"
#include "WindowsFunctionality.h"

Keyboard::Keyboard()
{
	// prevent accidentally reading bullshit values before the first keyboard poll runs
	keystates.reset();

	poll_thread = std::thread(&Keyboard::PollKeyboard, this);
}

Keyboard::~Keyboard()
{
	end_polling.store(true);
	poll_thread.join();
}

bool Keyboard::IsKeyPressed(int vk_code)
{
	return ReadWriteKeystates(true, vk_code);
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

void Keyboard::PollKeyboard()
{
	while (!end_polling.load())
	{
		ReadWriteKeystates(false);
		std::this_thread::sleep_for(polling_period);
	}
}

bool Keyboard::ReadWriteKeystates(bool is_read, int vk_code)
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
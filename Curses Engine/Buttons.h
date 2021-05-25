#pragma once

#include <string>

class Buttons
{
public:
	inline static const std::u8string Play = u8"PLAY";
	inline static const std::u8string Quit = u8"QUIT";
	inline static const std::u8string TryAgain = u8"TRY AGAIN";
	inline static const std::u8string Continue = u8"CONTINUE";
	inline static const std::u8string Restart = u8"RESTART";
	inline static const std::u8string MainMenu = u8"MAIN MENU";
	inline static const std::u8string Settings = u8"SETTINGS";
	inline static const std::u8string Back = u8"BACK";
	inline static const std::u8string WrapSettingOn = u8"WRAP AROUND: ON";
	inline static const std::u8string WrapSettingOff = u8"WRAP AROUND: OFF";
	inline static const std::u8string SnakeSpeed = u8"SNAKE SPEED: ";
	inline static const std::u8string Slow = u8"SLOW";
	inline static const std::u8string Normal = u8"NORMAL";
	inline static const std::u8string Fast = u8"FAST";
};
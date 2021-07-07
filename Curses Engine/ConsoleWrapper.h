#pragma once

#include "WindowsFunctionality.h"
#include "ExceptionBase.h"

class Console
{
public:
	// conpx (console pixel) is two characters on the same line
	// it is a square with a side of 2 * fontWidth
	typedef uint16_t conpx_count;	// represents amount of conpx's
	typedef uint16_t scrpx_count;	// represents amount of actual screen pixels
	typedef uint16_t char_count;	// represents amount of characters

	Console(scrpx_count fontWidthPx = 15u, std::wstring title = L"Curses Engine");
	Console(const Console&) = delete;
	Console(Console&&) = delete;
	Console& operator=(const Console&) = delete;
	Console& operator=(Console&&) = delete;
	~Console() {}
public:
	class Exception : public ExceptionBase
	{
	public:
		Exception(std::string funcName, std::string errorDesc,
			std::string fileName, int line) noexcept
			: ExceptionBase{ funcName, errorDesc, fileName, line }
		{
			errorType = "Console error";
		}
	};
public:
	static constexpr conpx_count widthConPx = 60u;
	static constexpr conpx_count heightConPx = 30u;
	static constexpr char_count width = widthConPx * 2u;
	static constexpr char_count height = heightConPx;
private:
	HWND hConsole;
	HANDLE consoleHandle;
	std::wstring title;
	scrpx_count fontWidth = 15u;
	scrpx_count screenWidth = 1920u;
	scrpx_count screenHeight = 1080u;
	static constexpr scrpx_count maxWidth = 1800u;
	static constexpr unsigned maxAspectRatio = 2u;
private:
	inline static size_t instances = 0u;
};

#define THROW_CONSOLE_EXCEPTION(funcName, errorDesc) throw Console::Exception{(funcName), (errorDesc), __FILE__, __LINE__}
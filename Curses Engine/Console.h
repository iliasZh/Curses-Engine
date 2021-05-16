#pragma once
#include "IliasWin.h"
#include "ExceptionBase.h"

class Console
{
public:
	Console(unsigned fontWidthPx = 15, std::wstring title = L"Curses Engine");
	Console(const Console&) = delete;
	Console(Console&&) = delete;
	Console& operator=(const Console&) = delete;
	Console& operator=(Console&&) = delete;
	~Console() {}
public:
	class Exception : public ExceptionBase
	{
	public:
		Exception(std::string funcname, std::string errorDesc,
				  std::string filename, int line) noexcept
			: ExceptionBase{ funcname, errorDesc, filename, line }
		{
			errorType = "Console error";
		}
	};
private:
	HWND hConsole;
	HANDLE consoleHandle;
	std::wstring title;
	unsigned fontWidthPx;
	unsigned screenWidthPx = 1920u;
	unsigned screenHeightPx = 1080u;
	unsigned maxHeightPx = 1000u;
	static constexpr unsigned maxAspectRatio = 2u;
public:
	static constexpr unsigned widthConPx = 60u;
	static constexpr unsigned heightConPx = 30u;
};

#define THROW_CONSOLE_EXCEPTION(func, errorDesc) throw Console::Exception{(func), (errorDesc), __FILE__, __LINE__}
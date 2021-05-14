#pragma once
#include "IliasWin.h"
#include "ExceptionBase.h"

class Console
{
public:
	Console(unsigned widthConPx = 120, unsigned heightConPx = 30, unsigned fontWidthPx = 15, std::wstring title = L"Curses Engine");
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
	unsigned widthConPx;
	unsigned heightConPx;
	unsigned fontWidthPx;
	unsigned screenWidthPx = 1920;
	unsigned screenHeightPx = 1080;
	float maxAspectRatio = 2.0f;
	unsigned maxHeightPx = 1000;
};

#define THROW_CONSOLE_EXCEPTION(func, errorDesc) throw Console::Exception{(func), (errorDesc), __FILE__, __LINE__}
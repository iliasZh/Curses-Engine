#pragma once

#include "WindowsFunctionality.h"
#include "ExceptionBase.h"

class Console
{
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
	// conpx (console pixel) is two characters on the same line
	// it is a square with a side of 2 * fontWidth
	typedef uint16_t conpx_count;	// represents amount of conpx's
	typedef uint16_t scrpx_count;	// represents amount of actual screen pixels
	typedef uint16_t char_count;	// represents amount of characters

	Console(char_count width,
			char_count height, 
			scrpx_count fontWidthPx = 15u,
			std::wstring title = L"WinCon Engine by Ilias");
	Console(scrpx_count fontWidthPx = 15u, std::wstring title = L"WinCon Engine by Ilias");
	Console(const Console&) = delete;
	Console(Console&&) = delete;
	Console& operator=(const Console&) = delete;
	Console& operator=(Console&&) = delete;
	~Console() {}
private:
	void SetupFont();
	void SetupScreenBuffer(bool maxSize);
	void SetTitleAndGetHwnd();
	void GetMonitorWorkAreaSize();
	void CenterWindow();
	void SetupStyle();
private:
	HWND hConsole;
	HANDLE conOut;
	std::wstring title;
	char_count width;
	char_count height;
	scrpx_count fontWidth;
	scrpx_count workAreaWidth = 1920u;
	scrpx_count workAreaHeight = 1080u;
private:
	inline static size_t instances = 0u;
};

#define THROW_CONSOLE_EXCEPTION(funcName, errorDesc) throw Console::Exception{(funcName), (errorDesc), __FILE__, __LINE__}
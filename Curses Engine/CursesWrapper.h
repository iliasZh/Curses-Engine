#pragma once

#include "WindowsFunctionality.h"
#include "ExceptionBase.h"

#undef MOUSE_MOVED

#include "curses.h"
#include "StringTypedefs.h"
#include <cassert>

class Curses
{
public:
	enum class CursorMode
	{
		// for curs_set(int) function
		Invisible,
		Normal,
		Full
	};
	enum class Color
	{
		Black	= COLOR_BLACK,
		Red		= COLOR_RED,
		Green	= COLOR_GREEN,
		Blue	= COLOR_BLUE,
		Yellow	= COLOR_YELLOW,
		Cyan	= COLOR_CYAN,
		Magenta = COLOR_MAGENTA,
		White	= COLOR_WHITE
	};
public:
	class Window
	{
	public:
		typedef uint16_t ucoord; // unsigned coordinate
		Window(ucoord startX, ucoord startY, ucoord width, ucoord height);
		~Window();
		Window(const Window&)				= delete;
		Window& operator=(const Window&)	= delete;

		void DrawBox(Color fg = Color::White, Color bg = Color::Black);
		void Refresh();
		void Touch();
		void Write(ucoord x, ucoord y, u8str_view str, Color fg = Color::White, Color bg = Color::Black);
		void GetCh();
		void Clear();
		
		ucoord GetStartX()	const { return startX; }
		ucoord GetStartY()	const { return startY; }
		ucoord Width()		const { return width; }
		ucoord Height()		const { return height; }
	private:
		ucoord startX, startY, width, height;
		WINDOW* win = nullptr;
	};
public:
	class Exception : public ExceptionBase
	{
	public:
		Exception(std::string funcName, std::string errorDesc,
			std::string fileName, int line) noexcept
			: ExceptionBase{ funcName, errorDesc, fileName, line }
		{
			errorType = "Curses error";
		}
	};
public:
	Curses();
	~Curses();
	Curses(const Curses&)				= delete;
	Curses(Curses&&)					= delete;
	Curses& operator=(const Curses&)	= delete;
	Curses& operator=(Curses&&)			= delete;

	void SetCursorMode(CursorMode mode);
	CursorMode GetCursorMode() { return cursorMode; }
	void SetEchoMode(bool enable);
	bool IsEchoEnabled() { return echoEnabled; }
	bool HasColors() { return has_colors(); }
	void Refresh() { refresh(); }
private:
	static bool IsInitialized() { return (instances == 1u); }
	static chtype GetColorPair(Color f, Color b);
private:
	CursorMode cursorMode = CursorMode::Normal;
	bool echoEnabled = true;
	inline static uint8_t instances = 0u; // instance counter, used to prevent creating multiple instances
private:
	static constexpr uint8_t numOfColors = 8u;
};

#define THROW_CURSES_EXCEPTION(func_name, error_desc) throw Curses::Exception{ (func_name), (error_desc), __FILE__, __LINE__ }
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
	Curses();
	~Curses();
	Curses(const Curses&)				= delete;
	Curses(Curses&&)					= delete;
	Curses& operator=(const Curses&)	= delete;
	Curses& operator=(Curses&&)			= delete;

	void SetCursorMode(CursorMode mode);
	CursorMode GetCursorMode() { return cursorMode; }
	void SetEchoMode(bool enable);
	bool IsEchoEnabled() { return echoMode; }
	bool HasColors() { return has_colors(); }
	void Refresh() { refresh(); }
	class Window
	{
	public:
		typedef uint16_t ucoord; // unsigned coordinate
		
		friend Curses::Curses(); // curses ctor initializes stdscreen window
		
		Window(ucoord startX, ucoord startY, ucoord width, ucoord height);
		~Window();
		Window(const Window&)				= delete;
		Window& operator=(const Window&)	= delete;
		Window& operator=(Window&& rhs) noexcept; // rhs becomes UNUSABLE after moving

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
		Window() 
			: startX{ 0u }, startY{ 0u }
			, width { (ucoord)getmaxx(stdscr) }
			, height{ (ucoord)getmaxy(stdscr) }
			, win{ stdscr }
		{}
	private:
		ucoord startX, startY, width, height;
		WINDOW* win = nullptr;
	};
public:
	class Exception : public ExceptionBase
	{
	public:
		Exception(std::string_view funcName, std::string_view errorDesc,
			std::string_view fileName, int line) noexcept
			: ExceptionBase{ funcName, errorDesc, fileName, line }
		{
			errorType = "Curses error";
		}
	};
public:
	Window stdwin;
private:
	static bool IsInitialized() { return (instances == 1u); }
	static chtype GetColorPair(Color f, Color b);
private:
	CursorMode cursorMode = CursorMode::Normal;
	bool echoMode = true;
	inline static uint8_t instances = 0u; // instance counter, used to prevent creating multiple instances
private:
	static constexpr uint8_t numOfColors = 8u;
};

#define THROW_CURSES_EXCEPTION(func_name, error_desc) throw Curses::Exception{ (func_name), (error_desc), __FILE__, __LINE__ }
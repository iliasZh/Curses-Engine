#pragma once

#include "IliasWin.h"
#include "ExceptionBase.h"

#undef MOUSE_MOVED

#include "curses.h"
#include <string>
#include <cassert>
#include <unordered_map>

namespace curses
{
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
			Black = COLOR_BLACK,
			Red = COLOR_RED,
			Green = COLOR_GREEN,
			Blue = COLOR_BLUE,
			Yellow = COLOR_YELLOW,
			Cyan = COLOR_CYAN,
			Magenta = COLOR_MAGENTA,
			White = COLOR_WHITE
		};
	private:
		class Window
		{
		public:
			//typedef typename Curses::Color Color;
			Window(int startX, int startY, int width, int height)
				: startX{ startX }
				, startY{ startY }
				, width{ width }
				, height{ height }
				, win{ newwin(height, width, startY, startX) }
			{
				assert(startX >= 0 && startY >= 0);
				assert(width > 0 && height > 0);
			}
			Window(const Window&) = delete;
			Window& operator=(const Window&) = delete;
			~Window();
			void DrawBox(Color fg = Color::White, Color bg = Color::Black);
			void Refresh();
			void Touch();
			void Write(int x, int y, std::u8string str, Color fg = Color::White, Color bg = Color::Black);
			void GetCh();
			void Clear();
			int GetCursorX();
			int GetCursorY();
		private:
			int startX, startY, width, height;
			WINDOW* win = nullptr;
		};
	public:
		class Exception : public ExceptionBase
		{
		public:
			Exception(std::string funcname, std::string errorDesc,
						std::string filename, int line) noexcept
				: ExceptionBase{ funcname, errorDesc, filename, line }
			{
				errorType = "Curses error";
			}
		};
	public:
		Curses();
		Curses(const Curses&) = delete;
		Curses(Curses&&) = delete;
		Curses& operator=(const Curses&) = delete;
		Curses& operator=(Curses&&) = delete;
		~Curses();

		// creates a window in 'windows' unordered map
		// if a window with specified name already exists, throws an exception
		void AddWindow(std::string name, int startX, int startY, int width, int height);
		
		// get a ref to window
		// throws an exception if window does not exist
		Window& operator[](std::string name);
		
		// returns true if a window was erased, false otherwise
		bool DeleteWindow(std::string name);

		void SetCursorMode(CursorMode mode);
		CursorMode GetCursorMode() { return cursorMode; }
		void SetEchoMode(bool enable);
		bool IsEchoEnabled() { return echoEnabled; }
		bool HasColors() { return has_colors(); }
	private:
		static chtype GetColorPair(Color f, Color b);
	private:
		std::unordered_map<std::string, Window> windows;
		CursorMode cursorMode = CursorMode::Normal;
		bool echoEnabled = true;
		inline static int instances = 0; // instance counter, used to prevent creating multiple instances
	private:
		static constexpr int numOfColors = 8;
	};

}

#define THROW_CURSES_EXCEPTION(func, errorDesc) throw Curses::Exception{ (func), errorDesc, __FILE__, __LINE__ }
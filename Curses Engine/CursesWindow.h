#pragma once

#include "IliasWin.h"

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
				assert(startX > 0 && startY > 0);
				assert(width > 0 && height > 0);
			}
			Window(const Window&) = delete;
			Window& operator=(const Window&) = delete;
			~Window();
			void DrawBox(Color fg = Color::White, Color bg = Color::Black);
			void Refresh();
			void Touch();
			void Write(int x, int y, std::u8string str, Color c = Color::White, Color bg = Color::Black);
			void GetCh();
			void Clear();
			int GetCursorX();
			int GetCursorY();
		private:
			int startX, startY, width, height;
			WINDOW* win = nullptr;
		};
	public:
		class Exception
		{
		public:
			Exception(std::wstring funcname, std::wstring errorDesc,
						std::wstring filename, int line) noexcept
				: funcname{ funcname }
				, errorDesc{ errorDesc }
				, filename{ filename }
				, line{ line }
			{}
			const wchar_t* what() const noexcept;
		private:
			std::wstring funcname;
			std::wstring errorDesc;
			std::wstring filename;
			int line;
			mutable std::wstring whatBuffer;
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
		static inline int instances = 0; // instance counter, used to prevent creating multiple instances
		static constexpr int numOfColors = 8;
	};

}

#define THROW_CURSES_EXCEPTION(func, errorDesc) throw Curses::Exception{ (func), errorDesc, WFILE, __LINE__ }
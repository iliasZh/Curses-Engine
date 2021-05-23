#pragma once

#include "IliasWin.h"
#include "ExceptionBase.h"

#undef MOUSE_MOVED

#include "curses.h"
#include <string>
#include <cassert>

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
	public:
		class Window
		{
		public:
			Window(int startX, int startY, int width, int height);
			Window(const Window&) = delete;
			Window& operator=(const Window&) = delete;
			~Window();
			void DrawBox(Color fg = Color::White, Color bg = Color::Black);
			void Refresh();
			void Touch();
			void Write(int x, int y, std::u8string str, Color fg = Color::White, Color bg = Color::Black);
			void GetCh();
			void Clear();
			int GetStartX() const;
			int GetStartY() const;
			int Width() const { return width; }
			int Height() const { return height; }
		protected:
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

		void SetCursorMode(CursorMode mode);
		CursorMode GetCursorMode() { return cursorMode; }
		void SetEchoMode(bool enable);
		bool IsEchoEnabled() { return echoEnabled; }
		bool HasColors() { return has_colors(); }

		static bool IsInitialized() { return (instances == 1); }
		static chtype GetColorPair(Color f, Color b);
	private:
		CursorMode cursorMode = CursorMode::Normal;
		bool echoEnabled = true;
		inline static int instances = 0; // instance counter, used to prevent creating multiple instances
	private:
		static constexpr int numOfColors = 8;
	};

}

#define THROW_CURSES_EXCEPTION(func, errorDesc) throw Curses::Exception{ (func), errorDesc, __FILE__, __LINE__ }
#pragma once

#include "IliasWin.h"

#undef MOUSE_MOVED

#include "curses.h"
#include <string>
#include <cassert>
#include <unordered_map>
#include <optional>
#include <exception>
#include <sstream>


namespace curses
{
	class Window
	{
	public:
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
		~Window()
		{
			// clear box and contents
			std::string fill(width, ' ');
			for (int i = 0; i < height; ++i)
			{
				mvwprintw(win, 0, i, fill.c_str());
			}
			wrefresh(win);
			//wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
			delwin(win);
		}
		void DrawBox()
		{
			box(win, 0, 0);
			wrefresh(win);
		}
		void Write(int x, int y, std::string str)
		{
			assert(x >= 0 && x < width);
			assert(y >= 0 && y < height);
			assert(str.length() < width - x);
			mvwprintw(win, x, y, str.c_str());
			wrefresh(win);
		}
		void GetCh()
		{
			flushinp();
			wgetch(win);
		}
	private:
		int startX, startY, width, height;
		WINDOW* win = nullptr;
	};

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

		Curses()
		{
			initscr();
			SetEchoMode(echoEnabled);
			SetCursorMode(cursorMode);
		}
		Curses(const Curses&) = delete;
		Curses& operator=(const Curses&) = delete;
		~Curses()
		{
			endwin();
		}
		void AddWindow(std::wstring name, int startX, int startY, int width, int height);
		Window& GetWindow(std::wstring name);
		void SetCursorMode(CursorMode mode);
		CursorMode GetCursorMode() { return cursorMode; }
		void SetEchoMode(bool enable);
		bool IsEchoEnabled() const { return echoEnabled; }
		bool HasColors() const { return has_colors(); }
	private:
		std::unordered_map<std::wstring, Window> windows;
		CursorMode cursorMode = CursorMode::Normal;
		bool echoEnabled = true;
	};

}

#define THROW_CURSES_EXCEPTION(func, errorDesc) throw Curses::Exception{ (func), errorDesc, WFILE, __LINE__ }

/*
	WINDOW* win;
	win = newwin(3, 10, (LINES - 3) / 2, (COLS - 10) / 2);
	box(win, 0, 0);
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	mvwprintw(win, 1, 1, "YOU DIED");
	wrefresh(win);
	wattroff(win, COLOR_PAIR(1));
	flushinp();
	wgetch(win);
	mvwprintw(win, 1, 1, "        ");
	wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(win);
	delwin(win);
*/
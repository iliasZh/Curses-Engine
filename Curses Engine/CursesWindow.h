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
	private:
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
			~Window();
			void DrawBox();
			void Write(int x, int y, std::u8string str);
			void GetCh();
		private:
			int startX, startY, width, height;
			WINDOW* win = nullptr;
		};
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

		Curses();
		Curses(const Curses&) = delete;
		Curses& operator=(const Curses&) = delete;
		~Curses();
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
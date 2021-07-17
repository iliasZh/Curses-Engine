#pragma once

#include "WindowsFunctionality.h"
#include "ExceptionBase.h"
#include "CharInfo.h"
#include <cassert>

class Console;

class Window
{
public:
	using Color = char_info::Color;
	using ColorPair = char_info::ColorPair;
	class Buffer
	{
	public:
		Buffer(USHORT width, USHORT height);
		~Buffer() noexcept { delete[] data; }
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;
		Buffer(Buffer&& buf) noexcept;
		Buffer& operator=(Buffer&& buf) noexcept;
		CHAR_INFO& At(USHORT x, USHORT y);
		void Clear(Color);
		COORD Size() const { return { (SHORT)width, (SHORT)height }; }
		const CHAR_INFO* Data() const { return data; }
	private:
		USHORT width;
		USHORT height;
		CHAR_INFO* data;
	};
public:
	Window(const Console& con, USHORT startX, USHORT startY, USHORT width, USHORT height);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) noexcept;
	Window& operator=(Window&&) noexcept;
	void Write(USHORT x, USHORT y, std::wstring_view text, Color fg, Color bg);
	void Write(USHORT x, USHORT y, std::wstring_view text, ColorPair col)
	{
		Write(x, y, text, col.fg, col.bg);
	}
	void WriteChar(USHORT x, USHORT y, wchar_t ch, Color fg, Color bg);
	void WriteChar(USHORT x, USHORT y, wchar_t ch, ColorPair col)
	{
		WriteChar(x, y, ch, col.fg, col.bg);
	}
	void DrawBox(Color c = Color::White);
	void Render();
	void Clear() { buf.Clear(bgColor); }
	void SetDefaultBgColor(Color c) { bgColor = c; }
	USHORT Width() const { return buf.Size().X; }
	USHORT Height() const { return buf.Size().Y; }
private:
	const Console& con;
	COORD startPos;
	Buffer buf;
	Color bgColor = Color::Black;
};

class Console
{
public:
	class Exception : public ExceptionBase
	{
	public:
		Exception(std::string_view funcName, std::string_view errorDesc,
			std::string_view fileName, int line) noexcept
			: ExceptionBase{ funcName, errorDesc, fileName, line }
		{
			errorType = "Console error";
		}
	};
public:
	enum class Cursor
	{
		Full,
		Underline,
		Invisible
	};
	typedef uint16_t px_count;	// represents amount of actual screen pixels

	Console(USHORT width, USHORT height, 
			px_count fontWidthPx = 15u,
			std::wstring_view title = L"Console Engine by Ilias");
	Console(px_count fontWidthPx = 15u, std::wstring_view title = L"Console Engine by Ilias");
	Console(const Console&) = delete;
	Console(Console&&) = delete;
	Console& operator=(const Console&) = delete;
	Console& operator=(Console&&) = delete;
	~Console() { --instances; }

	void Render(const CHAR_INFO* buffer, COORD size, COORD drawStart) const;
	void SetCursorMode(Cursor mode);
public:
	Window& Stdwin() { return *(pStdwin.get()); }
	bool IsInitialized() const { return instances == 1u; }
	USHORT Width() const { return width; }
	USHORT Height() const { return height; }
private:
	void SetupConsole(bool maxSize);
	void SetupFont();
	void SetupScreenBuffer(bool maxSize);
	void SetTitleAndGetHwnd();
	void GetMonitorWorkAreaSize();
	void CenterWindow();
	void SetupStyle();
private:
	HWND hConsole = NULL;
	HANDLE conOut = INVALID_HANDLE_VALUE;
	USHORT width;
	USHORT height;
	px_count fontWidth;
	std::wstring_view title;
	px_count workAreaWidth = 1920u;
	px_count workAreaHeight = 1080u;
	Cursor cursorMode = Cursor::Underline;
	std::unique_ptr<Window> pStdwin;
private:
	inline static size_t instances = 0u;
};

#define THROW_CONSOLE_EXCEPTION(funcName, errorDesc) throw Console::Exception{(funcName), (errorDesc), __FILE__, __LINE__}
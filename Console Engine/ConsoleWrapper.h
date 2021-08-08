#pragma once

#include "precompiled.h"
#include "ExceptionBase.h"
#include "CharInfo.h"
#include <cassert>

class Console;

class Window
{
public:
	class Buffer
	{
	public:
				Buffer(dimensions);
				~Buffer()					noexcept { delete[] data; }
				Buffer(const Buffer&)					= delete;
		Buffer& operator=(const Buffer&)				= delete;
				Buffer(Buffer&& buf)		noexcept;
		Buffer&	operator=(Buffer&& buf)		noexcept;
		
		CHAR_INFO&			At(ucoord);
		void				Clear(Color);
		const dimensions&	Dimensions() const { return dims; }
		const CHAR_INFO*	Data() const { return data; }
	private:
		dimensions	dims;
		CHAR_INFO*	data;
	};
public:
			Window(const Console&, ucoord, dimensions);
			Window(const Window&)					= delete;
	Window&	operator=(const Window&)				= delete;
			Window(Window&&)			noexcept;
	Window&	operator=(Window&&)			noexcept;

	void Write(ucoord, std::wstring_view, ColorPair);
	void WriteChar(ucoord, wchar_t, ColorPair);
	void DrawBox(ColorPair);
	void DrawBox(Color c)								{ DrawBox({ c, bgColor }); }
	void Clear()										{ buf.Clear(bgColor); }
	void SetDefaultBgColor(Color c)						{ bgColor = c; }

	void			Render()		const;
	const Console&	GetConsole()	const	{ return con; }
	uint16_t			Left()			const	{ return startPos.x; }
	uint16_t			Top()			const	{ return startPos.y; }
	uint16_t			Right()			const	{ return Left() + Width() - 1u; }
	uint16_t			Bottom()		const	{ return Top() + Height() - 1u; }
	uint16_t			Width()			const	{ return buf.Dimensions().w; }
	uint16_t			Height()		const	{ return buf.Dimensions().h; }
private:
	const Console&	con;
	ucoord			startPos;
	Buffer			buf;
	Color			bgColor = Color::Black;
};

class Console
{
public:
	class Exception : public ExceptionBase
	{
	public:
		Exception(
			std::string_view func_name_, std::string_view error_desc_,
			std::string_view file_name_, int line_) noexcept
			: ExceptionBase{ func_name_, error_desc_, file_name_, line_ }
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

				Console(dimensions dims_, px_count font_width_ = 15u,
						std::wstring_view title_ = L"Console Engine by Ilias");
				Console(px_count font_width_ = 15u,
						std::wstring_view title_ = L"Console Engine by Ilias");
				Console(const Console&)		= delete;
				Console(Console&&)			= delete;
	Console&	operator=(const Console&)	= delete;
	Console&	operator=(Console&&)		= delete;
				~Console() { --instances; }

	void SetCursorMode(Cursor);
	void Render(const CHAR_INFO* buf, ucoord, dimensions) const;
public:
	Window&		Stdwin()				{ return *(pStdwin.get()); }
	bool		IsInitialized() const	{ return instances == 1u; }
	uint16_t		Width()			const	{ return dims.w; }
	uint16_t		Height()		const	{ return dims.h; }
	HWND		WindowHandle()	const	{ return hConsole; }
	px_count	FontWidth()		const	{ return fontWidth; }
private: // setup methods
	void SetupConsole(bool maximize_size);
	void SetupFont();
	void SetupScreenBuffer(bool maximize_size);
	void SetTitleAndGetHwnd();
	void GetMonitorWorkAreaSize();
	void CenterWindow();
	void SetupStyle();
	void DisableTextSelection();
private:
	HWND					hConsole		= NULL;
	HANDLE					conOut			= INVALID_HANDLE_VALUE;
	HANDLE					conIn			= INVALID_HANDLE_VALUE;
	dimensions				dims;
	px_count				fontWidth;
	px_count				workAreaWidth	= 1920u;
	px_count				workAreaHeight	= 1080u;
	std::wstring_view		title;
	Cursor					cursorMode		= Cursor::Underline;
	std::unique_ptr<Window>	pStdwin;
private:
	inline static size_t	instances		= 0u;
};

#define THROW_CONSOLE_EXCEPTION(funcName, errorDesc) throw\
		Console::Exception{(funcName), (errorDesc), __FILE__, __LINE__}
#include "precompiled.h"
#include "ConsoleWrapper.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Utilities.h"

#include <iostream>

int main()
{
	try
	{
		Console con{ 15u };
		con.SetCursorMode(Console::Cursor::Invisible);
		con.Stdwin().DrawBox(Color::Cyan);
		con.Stdwin().Write({ 60u, 15u }, L"MIDDLE TEXT", { Color::Black, Color::Cyan });
		con.Stdwin().Write({ 60u, 16u }, L"MIDDLE TEXT", { Color::Black, Color::DarkCyan });
		con.Stdwin().Render();
		Sleep(5000);
	}
	catch (ExceptionBase& e)
	{
		MessageBox(nullptr, to_ws(e.what()).c_str(), to_ws(e.error_type()).c_str(), MB_OK | MB_ICONERROR);
	}
	catch (std::exception& e)
	{
		MessageBox(nullptr, to_ws(e.what()).c_str(), L"Standard C++ exception", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBox(nullptr, L"No details available", L"Unknown exception", MB_OK | MB_ICONERROR);
	}
	return 0;
}
#pragma once

#include <string>

inline std::wstring to_ws(const char* s)
{
	std::wstring ws;
	for (char* pc = const_cast<char*>(s); *pc != '\0'; ++pc)
	{
		ws.push_back(wchar_t(*pc));
	}
	return ws;
}
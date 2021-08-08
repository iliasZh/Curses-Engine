#pragma once

#include <string>
#include <utility>

inline std::wstring s_to_ws(std::string s)
{
	std::wstring ws(s.length(), L' ');
	std::copy(s.begin(), s.end(), ws.begin());
	return ws;
}

inline std::wstring to_ws(const char* s)
{
	return s_to_ws(std::move( std::string(s) ));
}
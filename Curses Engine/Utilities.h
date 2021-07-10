#pragma once

#include <string>
#include <utility>
#include <algorithm>

inline std::wstring s_to_ws(std::string s)
{
	std::wstring ws(s.length(), L' ');
	std::copy(s.begin(), s.end(), ws.begin());
	return ws;
}

inline std::wstring to_ws(const char* s)
{
	return s_to_ws(std::move(std::string(s)));
}

// may not work properly, i don't know shit about unicode
inline size_t count_codepoints(const u8str_view& str)
{
	size_t count = 0u;
	for (const auto& c : str)
		if ((c & 0b1100'0000) != 0b1000'0000) // Not a trailing byte
			++count;
	return count;
}
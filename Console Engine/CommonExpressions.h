#pragma once

#include "Expression.h"

class CommonExpressions
{
public:
	inline static const Expression menu = { {Lang::EN, L"MENU"}, {Lang::RU, L"����"}};
	inline static const Expression start = { {Lang::EN, L"START"}, {Lang::RU, L"������"}};
	inline static const Expression play = { {Lang::EN, L"PLAY"}, {Lang::RU, L"������"}};
	inline static const Expression settings = { {Lang::EN, L"SETTINGS"}, {Lang::RU, L"���������"}};
	inline static const Expression quit = { {Lang::EN, L"QUIT"}, {Lang::RU, L"�����"}};
	inline static const Expression language = { {Lang::EN, L"LANGUAGE"}, {Lang::RU, L"����"}};
	inline static const Expression english = { {Lang::EN, L"ENGLISH"}, {Lang::RU, L"����������"}};
	inline static const Expression russian  = { {Lang::EN, L"RUSSIAN"}, {Lang::RU, L"�������"}};
	inline static const Expression on  = { {Lang::EN, L"ON"}, {Lang::RU, L"���"}};
	inline static const Expression off  = { {Lang::EN, L"OFF"}, {Lang::RU, L"����"}};
	inline static const Expression small  = { {Lang::EN, L"SMALL"}, {Lang::RU, L"���."}};
	inline static const Expression medium  = { {Lang::EN, L"MEDIUM"}, {Lang::RU, L"����."}};
	inline static const Expression large  = { {Lang::EN, L"LARGE"}, {Lang::RU, L"�����."}};
};
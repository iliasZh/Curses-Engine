#pragma once

#include "WindowsFunctionality.h"
#include <string_view>

class Controls
{
public:
	class Binding
	{
	public:
		Binding(std::wstring_view name_, int primary_, int secondary_ = 0)
			: name{ name_ }, primary{ primary_ }, secondary{ secondary_ } {}
		
		std::wstring_view	GetName()	const { return name; }
		int					Primary()	const { return primary; }
		int					Secondary()	const { return secondary; }
	private:
		std::wstring_view	name;
		int					primary		= 0;
		int					secondary	= 0;
	};
public:
	inline static const Binding Up		{ L"Up",		'W',		VK_UP		};
	inline static const Binding Down	{ L"Down",		'S',		VK_DOWN		};
	inline static const Binding Left	{ L"Left",		'A',		VK_LEFT		};
	inline static const Binding Right	{ L"Right",		'D',		VK_RIGHT	};
	inline static const Binding Back	{ L"Back",		VK_ESCAPE,	VK_BACK		};
	inline static const Binding Select	{ L"Select",	'F',		VK_RETURN	};
};
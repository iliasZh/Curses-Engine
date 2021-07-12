#pragma once

#include "WindowsFunctionality.h"
#include "StringTypedefs.h"

class Controls
{
public:
	class Binding
	{
	public:
		Binding(u8str_view name, int vkcodeP, int vkcodeS = 0)
			: name{ name }
			, primary{ vkcodeP }
			, secondary{ vkcodeS }
		{}
		int Primary() const { return primary; }
		int Secondary() const { return secondary; }
		u8str_view GetName() const { return name; }
	private:
		u8str_view name;
		int primary = 0;
		int secondary = 0;
	};
public:
	inline static const Binding Up		{ u8"Up",		'W', VK_UP };
	inline static const Binding Down	{ u8"Down",		'S', VK_DOWN };
	inline static const Binding Left	{ u8"Left",		'A', VK_LEFT };
	inline static const Binding Right	{ u8"Right",	'D', VK_RIGHT };
	inline static const Binding Back	{ u8"Back",		VK_ESCAPE, VK_BACK };
	inline static const Binding Select	{ u8"Select",	'F', VK_RETURN };
};
#pragma once

#include "IliasWin.h"

class Controls
{
public:
	class Binding
	{
	public:
		Binding(std::u8string name, int vkcodeP, int vkcodeS = 0)
			: name{ name }
			, primary{ vkcodeP }
			, secondary{ vkcodeS }
		{}
		int Primary() const { return primary; }
		int Secondary() const { return secondary; }
		std::u8string GetName() const { return name; }
	private:
		std::u8string name;
		int primary = 0;
		int secondary = 0;
	};
public:
	inline static Binding Up{ u8"Up", 'W', VK_UP };
	inline static Binding Down{ u8"Down", 'S', VK_DOWN };
	inline static Binding Left{ u8"Left", 'A', VK_LEFT };
	inline static Binding Right{ u8"Right", 'D', VK_RIGHT };
	inline static Binding Back{ u8"Back", VK_ESCAPE, VK_BACK };
	inline static Binding Select{ u8"Select", 'F', VK_RETURN };
};
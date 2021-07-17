#pragma once

#include "Menu.h"

class TestMenu : public Menu
{
public:
	TestMenu(const Window& win, Keyboard& kbd, Expression title,
		EntryList entryList, LayoutDesc ld = {}, MenuPalette mp = {})
		: Menu{win, kbd, title, std::move(entryList), ld, mp }
	{}

	void OnSwitch(std::wstring_view name, std::wstring_view opt) override
	{
		using ex = CommonExpressions;
		if (name == ex::language)
		{
			if (opt == ex::english)
				global.lang = Lang::EN;
			else if (opt == ex::russian)
				global.lang = Lang::RU;

			parentWin.Render();
			CreateMenuWindow();
		}

		Expression menu_size = { {Lang::EN, L"MENU SIZE"}, {Lang::RU, L"–¿«Ã≈– Ã≈Õﬁ"} };
		if (name == menu_size)
		{
			if (opt == ex::small)
				SetLayoutDesc({ LayoutDesc::Pos::Centered, 1u, 0u });
			else if (opt == ex::medium)
				SetLayoutDesc({ LayoutDesc::Pos::Centered, 2u, 1u });
			else if (opt == ex::large)
				SetLayoutDesc({ LayoutDesc::Pos::Centered, 3u, 2u });
		}
		Refresh();
	}
	void OnSelect(std::wstring_view name) override
	{}
};
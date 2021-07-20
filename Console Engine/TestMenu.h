#pragma once

#include "Menu.h"

class TestMenu : public Menu
{
public:
	TestMenu(const Window& win, Keyboard& kbd)
		: Menu{ win, kbd, {{Lang::EN, L"TEST MENU"}, {Lang::RU, L"реярнбне лемч"}}, {{}}, {}, {} }
	{
		using ex = CommonExpressions;
		Expression menu_size = { {Lang::EN, L"MENU SIZE"}, {Lang::RU, L"пюглеп лемч"} };
		std::vector<std::unique_ptr<Entry>> entries;
		entries.push_back(MakeButton(ex::start,		false));
		entries.push_back(MakeSwitch(ex::language,	{ ex::english, ex::russian }));
		entries.push_back(MakeSwitch(menu_size,		{ ex::small, ex::medium, ex::large }));
		entries.push_back(MakeButton(ex::quit,		true));

		EntryList el{ std::move(entries) };

		SetEntryList(std::move(el));
	}

	void OnSwitch(std::wstring_view name, std::wstring_view opt) override
	{
		using ex = CommonExpressions;
		if (name == ex::language)
		{
			if (opt == ex::english)
				global::lang = Lang::EN;
			else if (opt == ex::russian)
				global::lang = Lang::RU;

			parentWin.Render();
			CreateMenuWindow();
		}

		Expression menu_size = { {Lang::EN, L"MENU SIZE"}, {Lang::RU, L"пюглеп лемч"} };
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
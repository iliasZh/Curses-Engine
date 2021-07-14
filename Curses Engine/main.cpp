#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "Game.h"
#include "RandomNumberGenerator.h"
#include "Menu.h"
#include "CommonExpressions.h"

int main()
{
	//Game game;
	Console con;
	Curses cs;
	Keyboard kbd;

	cs.SetEchoMode(false);
	cs.SetCursorMode(Curses::CursorMode::Invisible);

	Expression menu_size = { {Lang::EN, u8"MENU SIZE"}, {Lang::RU, u8"РАЗМЕР МЕНЮ"}};
	using ex = CommonExpressions;
	std::vector<std::unique_ptr<Entry>> en_ents{};
	en_ents.push_back(MakeButton(ex::start,		false)									);
	en_ents.push_back(MakeSwitch(ex::language,	{ ex::english, ex::russian })			);
	en_ents.push_back(MakeSwitch(menu_size,		{ ex::small, ex::medium, ex::large })	);
	en_ents.push_back(MakeButton(ex::quit,		true)									);

	EntryList el{ std::move(en_ents) };
	
	LayoutDesc ld(LayoutDesc::Pos::Centered, 1u, 0u);

	Menu menu(cs.stdwin, kbd, ex::menu, std::move(el), ld);
	menu.EnableLooping();
	//menu.EnableSwitchesLooping();
	menu.Refresh();
	menu.Listen();

	return 0;
}
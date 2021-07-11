#pragma comment(lib, "C:\\cpplibraries\\pdcurses.lib")

#include "Game.h"
#include "RandomNumberGenerator.h"
#include "Menu.h"

#include <iostream>

int main()
{
	//Game game;
	Console con;
	Curses cs;
	Keyboard kbd;

	cs.SetEchoMode(false);
	cs.SetCursorMode(Curses::CursorMode::Invisible);

	//EntryList el
	//{
	//	{
	//		MakeButton(u8"START",		false							),
	//		MakeButton(u8"SETTINGS",	true							),
	//		MakeButton(u8"ABOUT",		false							),
	//		MakeSwitch(u8"CHOOSE",		Entry::Options{u8"YES", u8"NO"}	),
	//		MakeButton(u8"QUIT",		true							)
	//	}
	//};

	std::vector<std::unique_ptr<Entry>> ents{};
	ents.push_back(MakeButton(u8"START",	false)								);
	ents.push_back(MakeButton(u8"SETTINGS", true)								);
	ents.push_back(MakeButton(u8"ABOUT",	false)								);
	ents.push_back(MakeSwitch(u8"CHOOSE",	Switch::Options{ u8"YES", u8"MAYBE", u8"NO"}));
	ents.push_back(MakeButton(u8"QUIT",		true)								);

	EntryList el{ std::move(ents) };

	LayoutDesc ld(LayoutDesc::Pos::Centered, 5u, 2u);

	Menu menu(cs.stdwin, kbd, u8"MENU", std::move(el), ld);
	menu.EnableLooping();
	menu.EnableSwitchesLooping();
	menu.Refresh();
	menu.Listen();

	return 0;
}
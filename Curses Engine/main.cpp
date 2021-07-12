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

	std::vector<std::unique_ptr<Entry>> en_ents{};
	en_ents.push_back(MakeButton(u8"START",		false)												);
	en_ents.push_back(MakeButton(u8"SETTINGS",	false)												);
	en_ents.push_back(MakeSwitch(u8"LANGUAGE",	Switch::Options{u8"ENGLISH", u8"РУССКИЙ"})			);
	en_ents.push_back(MakeSwitch(u8"MENU SIZE",	Switch::Options{ u8"SMALL", u8"MEDIUM", u8"LARGE"})	);
	en_ents.push_back(MakeButton(u8"QUIT",		true)												);

	EntryList el{ std::move(en_ents) };
	
	LayoutDesc ld(LayoutDesc::Pos::Centered, 1u, 1u);

	Menu menu(cs.stdwin, kbd, u8"MENU MENU MENU MENU MENU MENU", std::move(el), ld);
	menu.EnableLooping();
	//menu.EnableSwitchesLooping();
	menu.Refresh();
	menu.Listen();

	return 0;
}
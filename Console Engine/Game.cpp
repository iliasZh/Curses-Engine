#include "precompiled.h"
#include "Game.h"

Game::Game(Console::px_count font_width_, std::wstring_view title_)
	: console{ font_width_, title_ }
{}

void Game::Go()
{
	Logic();
	DrawFrame();
}

void Game::Logic()
{
}

void Game::DrawFrame()
{
}
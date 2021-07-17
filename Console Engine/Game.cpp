#include "Game.h"

Game::Game(Console::px_count fontWidth, std::wstring_view title)
	: console{ fontWidth, title }
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
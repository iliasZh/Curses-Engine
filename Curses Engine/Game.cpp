#include "Game.h"

Game::Game(Console::scrpx_count fontWidth, std::wstring title)
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
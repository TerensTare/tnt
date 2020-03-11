#include "Application.hpp"

tnt::Game::Game()
{
    timer = new Timer();
}

tnt::Game::~Game() noexcept
{
    delete timer;
    timer = nullptr;
}
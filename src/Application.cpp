#include "Application.hpp"

tnt::Game &tnt::Game::This()
{
    static Game game;
    return game;
}
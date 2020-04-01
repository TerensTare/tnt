#include "core/Context.hpp"
#include "fileIO/VirtualFS.hpp"
#include "core/Window.hpp"

tnt::Context::~Context() noexcept
{
    delete fs;
    fs = nullptr;

    delete window;
    window = nullptr;

    delete assets;
    assets = nullptr;
}

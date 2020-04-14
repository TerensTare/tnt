#include "core/Context.hpp"
#include "core/Window.hpp"
#include "fileIO/VirtualFS.hpp"

tnt::Context::~Context() noexcept
{
    delete fs;
    fs = nullptr;

    window.reset();

    delete assets;
    assets = nullptr;
}

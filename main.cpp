// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "core/Window.hpp"
#include "fileIO/Snipper.hpp"

using nlohmann::json;

auto read_to_json(std::string_view filename)
{
    json j;
    {
        std::ifstream file{filename.data()};
        file >> j;
    }
    return j;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    bool quit{false};
    json data{read_to_json("test.json")};
    SDL_Event e;

    tnt::Window *window{new tnt::Window{
        "Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE}};

    tnt::file::Snipper snipper;
    snipper.watchFile("test.json");

    while (!quit)
    {
        SDL_WaitEvent(&e);
        switch (e.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }

        snipper.onModify("test.json", [&]() -> void {
            data = read_to_json("test.json");
        });

        window->SetTitle(data["title"].get<std::string>().c_str());
        window->SetClearColor({data["r"], data["g"], data["b"], data["a"]});

        window->Clear();
        window->Render();
    }

    delete window;
    return 0;
}

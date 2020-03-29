// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "core/Window.hpp"
#include "fileIO/Snipper.hpp"
#include "core/InputManager.hpp"
#include "fileIO/AssetManager.hpp"
#include "utils/Timer.hpp"
#include "ecs/Sprite.hpp"

using nlohmann::json;

// TODO: "dissolve" this code into classes, like Game/Scene/Space, etc.

auto read_to_json = [](std::string_view filename) {
    json j;
    {
        std::ifstream file{filename.data()};
        file >> j;
    }
    return j;
};

class Player : public tnt::Sprite
{
public:
    explicit Player(tnt::Window const *win)
        : tnt::Sprite{
              win, ".\\bin\\x64\\release\\player.png",
              SDL_Rect{0, 0, 100, 100}} {}

    virtual void Update() override
    {
        std::cout << "updating player\n";
    }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    bool quit{false};
    json data{read_to_json("test.json")};
    SDL_Event e;

    tnt::Window *window{new tnt::Window{
        "The TnT Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE}};

    auto assets{tnt::AssetManager::This()};

    tnt::Sprite *player{new Player{window}};

    tnt::file::Snipper snipper;
    snipper.watchFile("test.json");

    tnt::InputManager &input{tnt::InputManager::This()};
    tnt::Timer timer;

    window->SetClearColor({data["r"], data["g"], data["b"], data["a"]});

    while (!quit)
    {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                quit = true;

        if (input.KeyPressed(SDL_SCANCODE_T))
            std::cout << "Pressed T\n";

        snipper.onModify("test.json", [&]() -> void {
            std::cout << "edited test.json\n";
            data = read_to_json("test.json");

            window->SetClearColor({data["r"], data["g"], data["b"], data["a"]});
            window->SetTitle(data["title"].get<std::string>().c_str());
        });

        player->Update();

        input.UpdatePreviousInput();
        input.UpdateCurrentInput();

        window->Clear();

        auto dst{SDL_FRect{0, 0, 100, 100}};
        player->getSprite()->Draw(window, dst);

        window->Render();

        auto fps{1000 / timer.deltaTime().count()};
        timer.reset();
        std::cout << fps << " fps\n";
        SDL_Delay(16);
    }
    delete window;
    return 0;
}

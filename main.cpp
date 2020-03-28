// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "core/Window.hpp"
#include "fileIO/Snipper.hpp"
#include "core/InputManager.hpp"
#include "exp/AssetManagerv2.hpp"
#include "utils/Timer.hpp"
#include "ecs/Sprite.hpp"

using nlohmann::json;

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
        : tnt::Sprite{win, ".\\bin\\x64\\release\\player.png"} {}

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
        "Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE}};

    auto assets{tnt::exp::AssetManager::This()};

    tnt::Sprite *player{new Player{window}};

    tnt::file::Snipper snipper;
    snipper.watchFile("test.json");

    tnt::InputManager &input{tnt::InputManager::This()};
    tnt::Timer timer;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                quit = true;

        if (input.KeyPressed(SDL_SCANCODE_T))
            std::cout << "Pressed T\n";

        snipper.onModify("test.json", [&]() -> void {
            std::cout << "edited test.json";
            data = read_to_json("test.json");
        });

        player->Update();

        window->SetClearColor({data["r"], data["g"], data["b"], data["a"]});
        window->SetTitle(data["title"].get<std::string>().c_str());

        input.UpdatePreviousInput();
        input.UpdateCurrentInput();

        window->Clear();

        auto dst{SDL_FRect{0, 0, 600, 400}};
        window->Draw(player, nullptr, &dst);

        window->Render();

        auto fps{1000 / timer.deltaTime().count()};
        timer.reset();
        std::cout << fps << " fps\n";
        SDL_Delay(16);
    }
    delete window;
    return 0;
}

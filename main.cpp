// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "exp/imgui/gui_config.hpp"
#include "exp/imgui/ImGui.hpp"

#include "ecs/Sprite.hpp"
#include "core/Context.hpp"
#include "core/Window.hpp"
#include "core/InputManager.hpp"
#include "fileIO/AssetManager.hpp"
#include "fileIO/Snipper.hpp"
#include "utils/Timer.hpp"
#include "math/Rectangle.hpp"
#include "math/Easings.hpp"

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
              tnt::Rectangle{0.f, 0.f, 0.f, 0.f}} {}

    virtual void Update(long long elapsed) noexcept override
    {
        std::cout << "updating player\n";
    }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    bool quit{false};
    long long dt{0};

    json data{read_to_json("test.json")};
    SDL_Event e;

    tnt::Window *window{new tnt::Window{
        "The TnT Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE}};

    tnt::ImGui::context_t *context{tnt::ImGui::make_context(window)};

    tnt::Rectangle dst{0, 0, 100, 100};

    tnt::Sprite *player{new Player{window}};

    tnt::file::Snipper snipper;
    snipper.watchFile("test.json");

    tnt::InputManager &input{tnt::InputManager::This()};
    tnt::Timer timer;

    window->setClearColor({data["r"], data["g"], data["b"], data["a"]});

    while (!quit)
    {
        tnt_imgui_begin(context);

        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                quit = true;

        // if (input.KeyPressed(SDL_SCANCODE_D))
        //     dst.x += 80.f;
        // else if (input.KeyPressed(SDL_SCANCODE_W))
        //     dst.y -= 80.f;
        // else if (input.KeyPressed(SDL_SCANCODE_A))
        //     dst.x -= 80.f;
        // else if (input.KeyPressed(SDL_SCANCODE_S))
        //     dst.y += 80.f;

        snipper.onModify("test.json", [&]() -> void {
            std::cout << "edited test.json\n";
            data = read_to_json("test.json");

            window->setClearColor({data["r"], data["g"], data["b"], data["a"]});
            window->setTitle(data["title"].get<std::string>().c_str());
        });

        player->Update(0);

        if (dt != 0)
        {
            dst.w = tnt::sine::EaseInOut(static_cast<float>(dt) / 1000.f, dst.w, 300 - dst.w, .1);
            dst.h = tnt::sine::EaseInOut(static_cast<float>(dt) / 1000.f, dst.h, 300 - dst.h, .1);
        }

        if (tnt::ImGui::button(context, IMGEN_WIDGET_ID(1), 300, 400))
            std::cout << "pressed button\n";

        input.UpdatePreviousInput();
        input.UpdateCurrentInput();

        tnt_imgui_finish(context);

        // window->Clear();
        tnt_imgui_draw(window);
        player->getSprite()->Draw(window, dst);

        // window->Render();

        dt = timer.deltaTime().count();
        timer.reset();
        std::cout << (1000 / (dt + 1)) << " fps\n";

        SDL_Delay(16);
    }

    delete player;

    tnt::ImGui::destroy_context(context);

    delete window;

    return 0;
}

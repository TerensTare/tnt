// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>

#include "imgui/gui_config.hpp"
#include "imgui/ImGui.hpp"

#include "ecs/Sprite.hpp"
#include "core/Window.hpp"
#include "core/InputManager.hpp"
#include "fileIO/AssetManager.hpp"
#include "utils/Timer.hpp"
#include "math/Rectangle.hpp"

using tnt::ImGui::button,
    tnt::ImGui::slider_int,
    tnt::ImGui::hslider_int;

// TODO: "dissolve" this code into classes, like Game/Scene/Space, etc.

auto random_name = []() -> std::string_view {
    static std::string_view names[]{
        "window", "test", "imgui", "the tnt engine", "hello world"};
    int idx{std::rand() % 4};
    return names[idx];
};

class Player : public tnt::Sprite
{
public:
    explicit Player(tnt::Window const *win)
        : tnt::Sprite{
              win, ".\\bin\\x64\\release\\player.png",
              tnt::Rectangle{0.f, 0.f, 100.f, 100.f}} {}

    virtual void Update(long long elapsed) noexcept override
    {
        std::cout << "updating player\n";
    }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    std::srand(static_cast<unsigned>(time(0)));
    bool quit{false};
    long long dt{0};

    SDL_Color bg{0, 0, 0, 255};
    SDL_Event e;

    tnt::Window *window{new tnt::Window{
        "The TnT Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE}};

    tnt::ImGui::make_context(1280, 720);

    SDL_Rect dst{0, 0, 100, 100};
    auto &input{tnt::InputManager::This()};

    tnt::Sprite *player{new Player{window}};

    tnt::Timer timer;

    window->setClearColor({0, 0, 0, 255});

    while (!quit)
    {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                quit = true;
        tnt::ImGui::update_context();

        input.UpdatePreviousInput();
        input.UpdateCurrentInput();

        window->Clear();

        tnt_imgui_begin();

        if (button(window, IMGEN_WIDGET_ID(1), 300, 400))
            window->setTitle(random_name().data());

        hslider_int(window, IMGEN_WIDGET_ID(2), 500, 100,
                    0, 400, dst.w);
        hslider_int(window, IMGEN_WIDGET_ID(3), 500, 140,
                    0, 400, dst.h);

        tnt::Rectangle area{dst.x, dst.y, dst.w, dst.h};

        tnt_imgui_finish();

        player->getSprite()->Draw(window, area);
        window->Render();

        dt = timer.deltaTime().count();
        timer.reset();

        if (!quit)
            std::cout << (1000 / dt) + 1 << " fps\n";

        SDL_Delay(16);
    }

    delete player;

    tnt::ImGui::destroy_context();

    delete window;

    return 0;
}

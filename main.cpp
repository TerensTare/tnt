// This is an independent project of an individual developer. Dear PVS-Studio,
// please check it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com

#include <iostream>
#include <random>

#include "core/InputManager.hpp"
#include "core/Window.hpp"
#include "ecs/Sprite.hpp"
#include "fileIO/AssetManager.hpp"
#include "imgui/ImGui.hpp"
#include "imgui/gui_config.hpp"
#include "math/Rectangle.hpp"
#include "utils/Timer.hpp"

using tnt::ImGui::button, tnt::ImGui::slider_int, tnt::ImGui::hslider_int;

// TODO: "dissolve" this code into classes, like Game/Scene/Space, etc.

auto random_name = []() -> std::string_view {
    static std::string_view names[]{"window", "test", "imgui", "the tnt engine",
                                    "hello world"};
    int idx{std::rand() % 4};
    return names[idx];
};

class Player : public tnt::Sprite
{
  public:
    explicit Player(tnt::Window const *win)
        : tnt::Sprite{
              win,
              std::move(std::string{SDL_GetBasePath()}.append("player.png")),
              tnt::Rectangle{0.f, 0.f, 208.f, 384.f}}
    {}

    void Update(long long elapsed) noexcept override
    {
        std::cout << "updating player\n";
    }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    std::random_device rd;
    std::srand(rd());
    bool quit{false};

    SDL_Color bg{0, 0, 0, 255};

    tnt::Window *window{new tnt::Window{
        "The TnT Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE}};

    window->setClearColor({10, 210, 255, 255});
    tnt::ImGui::make_context(window);

    SDL_Rect dst{0, 0, 150, 150};
    tnt::Sprite *player{new Player{window}};
    tnt::Timer timer;

    while (!quit)
    {
        long long dt{0};
        SDL_Event e;

        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                quit = true;
        tnt::ImGui::update_context();

        tnt::input::updatePrevious();
        tnt::input::updateCurrent();

        window->Clear();

        tnt_imgui_begin();

        if (button(window, IMGUI_ID, "Hello", 300, 400))
            window->setTitle(random_name().data());

        hslider_int(window, IMGUI_ID, 500, 100, 100, 400, &dst.w);
        hslider_int(window, IMGUI_ID, 500, 140, 100, 400, &dst.h);

        tnt::Rectangle area{dst.x, dst.y, dst.w, dst.h};

        tnt_imgui_finish();

        player->getSprite()->Draw(window, area);
        window->Render();

        dt = timer.deltaTime().count();
        timer.reset();

        if (!quit)
            std::cout << (1000 / dt) << " fps\n";

        SDL_Delay(1);
    }

    delete player;

    tnt::ImGui::destroy_context();
    tnt::input::close();

    delete window;

    return 0;
}

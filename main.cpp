// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Input.hpp"
#include "core/Window.hpp"
#include "core/Space.hpp"
#include "core/Scene.hpp"

#include "ecs/Sprite.hpp"

#include "fileIO/AssetManager.hpp"

#include "imgui/ImGui.hpp"

#include "utils/Logger.hpp"
#include "utils/Timer.hpp"

using tnt::ImGui::hslider_float, tnt::ImGui::hslider_float2,
    tnt::ImGui::hslider_vec, tnt::ImGui::WindowFlags;

// TODO: "dissolve" this code into classes, like Game/Scene/Space, etc.

class Player : public tnt::Sprite
{
public:
    explicit Player(tnt::Window const *win)
        : tnt::Sprite{win, "assets/player.png",
                      tnt::Rectangle{3.f, 0.f, 10.f, 16.f}} {}

    inline void Update(long long time_) noexcept override { return; }
};

int main(int, char **)
{
    tnt::Window *window{new tnt::Window{
        "The TnT Engine", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE}};

    tnt::Timer timer;

    Player *player{new Player{window}};
    player->setPosition(tnt::Vector{100.f, 160.f});
    player->setScale(tnt::Vector{10.f, 10.f});

    tnt::Space space;
    space.addObject("player", player);

    tnt::Scene editor{window, "assets/sky.png"};
    editor.addSpace("gameSpace", &space);

    long long dt{0};
    SDL_DisplayMode display;
    SDL_GetDisplayMode(0, 0, &display);

    tnt_imgui_init(window);

    while (window->isOpened())
    {
        dt = timer.deltaTime().count();
        timer.reset();

        auto [xScale, yScale]{player->getScale()};
        tnt::Vector params{(float)player->getWidth(), (float)player->getHeight()};
        tnt::Vector pos{player->getPosition()};

        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            window->handleEvents(e);
            if (e.type == SDL_WINDOWEVENT &&
                e.window.event == SDL_WINDOWEVENT_RESIZED)
                SDL_GetDisplayMode(0, 0, &display);

            tnt::ImGui::update_context();
        }

        if (dt != 0)
            editor.Update(dt);

        window->Clear();
        editor.Draw(window);

        if (tnt::ImGui::Begin(window, "Properties", 300, 300))
        {
            if (tnt::ImGui::BeginSection(window, "Transform"))
            {
                {
                    static float angle{360.f};
                    if (hslider_float(window, "Rotation", 0.f, 720.f, &angle))
                        player->setAngle(angle);
                }

                if (hslider_float2(window, "Scale", .5f, 100.f, &xScale, &yScale))
                    player->setScale(tnt::Vector{xScale, yScale});

                if (hslider_vec(window, "Position",
                                params.x / 2, (float)display.w - (params.x / 2),
                                params.y / 2, (float)display.h - (params.y / 2),
                                &pos))
                    player->setPosition(pos);

                tnt::ImGui::EndSection();
            }

            tnt::ImGui::button(window, "Test");

            tnt::ImGui::End();
        }

        window->Render();

        SDL_Delay(1);
    }

    tnt_imgui_close();
    tnt::input::close();

    delete player;
    delete window;

    return 0;
}
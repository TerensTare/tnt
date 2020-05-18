// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Input.hpp"
#include "core/Window.hpp"
#include "core/Camera.hpp"
#include "ecs/Sprite.hpp"
#include "fileIO/AssetManager.hpp"
#include "imgui/ImGui.hpp"
// #define TNT_IMGUI_RUNTIME_CONFIG
// #include "imgui/gui_config.hpp"
#include "utils/Logger.hpp"
#include "utils/Timer.hpp"

using tnt::ImGui::hslider_int, tnt::ImGui::menu_button, tnt::ImGui::menu_item;

// TODO: "dissolve" this code into classes, like Game/Scene/Space, etc.

class Player : public tnt::Sprite
{
public:
    explicit Player(tnt::Window const *win)
        : tnt::Sprite{win, std::move(std::string{SDL_GetBasePath()}.append("assets/player.png")),
                      tnt::Rectangle{0.f, 0.f, 16.f, 16.f}} {}

    inline void Update(long long) noexcept override { return; }
};

int main(int, char **)
{
    tnt::Window *window{new tnt::Window{
        "The TnT Engine", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE}};

    window->setClearColor({10, 210, 255, 255});
    tnt_imgui_init(window);

    tnt::FullTrackingCamera camera{0.f, 0.f, 160.f, 160.f};
    SDL_Rect clip{0, 0, 16, 16};
    tnt::Sprite *player{new Player{window}};
    tnt::Timer timer;

    int x{0}, y{0};
    long long dt{0};

    while (window->isOpened())
    {
        dt = timer.deltaTime().count();
        timer.reset();

        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            window->handleEvents(e);
            tnt::ImGui::update_context();

            if (tnt::input::keyDown(SDL_SCANCODE_D))
                camera.x = camera.x + dt * 20.f;
            if (tnt::input::keyDown(SDL_SCANCODE_A))
                camera.x = camera.x - dt * 20.f;
            if (tnt::input::keyDown(SDL_SCANCODE_W))
                camera.y = camera.y - 20.f * dt;
            if (tnt::input::keyDown(SDL_SCANCODE_S))
                camera.y = camera.y + 20.f * dt;
            if (tnt::input::keyDown(SDL_SCANCODE_Z))
            {
                camera.w = camera.w * 1.1f * dt; // zoom with 1.1
                camera.h = camera.h * 1.1f * dt;
            }
            if (tnt::input::keyDown(SDL_SCANCODE_X))
            {
                camera.w = camera.w * .091f * dt; // zoom with 0.9
                camera.h = camera.h * .091f * dt;
            }
        }

        if (dt != 0)
            player->Update(dt);

        window->Clear();

        window->Draw(player, clip, camera);

        if (tnt::ImGui::Begin(window, "Properties", 500, 300))
        {
            hslider_int(window, "x", 0, 9, &x);
            hslider_int(window, "y", 0, 3, &y);

            tnt::ImGui::End();
        }

        clip.x = x * 16;
        clip.y = y * 16;

        window->Render();

        // if (!quit)
        //     std::cout << dt << " fps\n";
        SDL_Delay(1);
    }

    delete player;

    tnt_imgui_close();
    tnt::input::close();

    return 0;
}

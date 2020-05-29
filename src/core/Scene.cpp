// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Scene.hpp"
#include "core/Space.hpp"
#include "core/Window.hpp"
#include "core/Camera.hpp"

#include "fileIO/AssetManager.hpp"

tnt::Scene::Scene(Window const *win, std::string_view background) noexcept
    : camera_{tnt::Rectangle{0, 0, win->getWidth(), win->getHeight()}}
{
    if (background == "")
        bg = nullptr;
    else
        bg = AssetManager::This().Image(win, background);
}

void tnt::Scene::LoadAssets(tnt::Window const *win, std::span<std::string_view> assets)
{
    AssetManager asset_man{AssetManager::This()};
    for (auto const &it : assets)
        if (it.ends_with(".png"))
            asset_man.AddImage(win, it);
        else if (it.ends_with(".mp3"))
            asset_man.AddMusic(it);
        else if (it.ends_with(".wav"))
            asset_man.AddSfx(it);
}

void tnt::Scene::LoadFont(std::string_view name, int size)
{
    AssetManager::This().AddFont(name, size);
}

void tnt::Scene::addSpace(std::string_view name, tnt::Space *space)
{
    spaces.try_emplace(name.data(), space);
}

tnt::Space *tnt::Scene::getSpace(std::string_view name) const noexcept { return spaces.at(name.data()); }

void tnt::Scene::Draw(tnt::Window const *win)
{
    camera_.w = static_cast<float>(win->getWidth());
    camera_.h = static_cast<float>(win->getHeight());

    const SDL_FRect dst{static_cast<SDL_FRect>(camera_.Bounds())};
    SDL_RenderCopyF(win->getRenderer(), bg, nullptr, &dst);
    for (auto const &it : spaces)
        it.second->Draw(win, camera_);
}

void tnt::Scene::Update(long long time_)
{
    for (auto const &it : spaces)
        it.second->Update(time_);
}
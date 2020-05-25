// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "core/Scene.hpp"
#include "core/Space.hpp"
#include "core/Window.hpp"
#include "core/Camera.hpp"

#include "fileIO/AssetManager.hpp"

tnt::Scene::Scene(std::shared_ptr<Window> const &window_) noexcept : window{window_} {}

tnt::Scene::~Scene() noexcept
{
    delete camera_;
    camera_ = nullptr;

    std::vector<Space *>{}.swap(spaces);
}

void tnt::Scene::LoadAssets(std::vector<std::string> const &assets)
{
    auto asset_man{AssetManager::This()};
    for (auto const &it : assets)
        if (it.ends_with(".png"))
            asset_man.AddImage(&*window.lock(), it);
        else if (it.ends_with(".mp3"))
            asset_man.AddMusic(it);
        else if (it.ends_with(".wav"))
            asset_man.AddSfx(it);
}

void tnt::Scene::LoadFont(std::string_view name, int size) noexcept
{
    AssetManager::This().AddFont(name, size);
}

void tnt::Scene::Draw()
{
    for (auto const &it : spaces)
        it->Draw(&*window.lock(), *camera_);
}

void tnt::Scene::Update(long long time_)
{
    for (auto const &it : spaces)
        it->Update(time_);
}
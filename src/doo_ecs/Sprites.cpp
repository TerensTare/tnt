// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Bones.hpp"
#include "doo_ecs/Cameras.hpp"
#include "doo_ecs/Objects.hpp"
#include "doo_ecs/Sprites.hpp"

#include "fileIO/AssetCache.hpp"
#include "json/JsonRectangle.hpp"

#include "utils/Benchmark.hpp"

namespace tnt::doo
{
    inline static medium_texture_cache *cache{default_texture_cache()};

    // sprites_comp
    sprite_comp::sprite_comp(Window const &win, std::string_view filename) noexcept
        : crop{0, 0, 0, 0}
    {
        tex = cache->get(win.getRenderer(), filename);
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        crop.w = w;
        crop.h = h;
    }

    sprite_comp::sprite_comp(Window const &win, std::string_view filename,
                             Rectangle const &rect) noexcept
        : crop{(int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h},
          w{(int)rect.w}, h{(int)rect.h}
    {
        tex = cache->get(win.getRenderer(), filename);
    }

    // sprites_sys
    void sprites_sys::add_object(object const &id, sprite_comp const &sprite)
    {
        safe_ensure(objects.active.contains(id), "Adding inexistent object to sprites_sys!!");

        PROFILE_FUNCTION();

        [[unlikely]] if (id >= tex.capacity())
        {
            tex.reserve(id - tex.capacity() + 1);
            clip.reserve(id - tex.capacity() + 1);
        }

        active.insert(id, id);
        tex.emplace(tex.cbegin() + id, sprite.tex);
        clip.emplace(clip.cbegin() + id, sprite.crop);
    }

    void sprites_sys::from_json(object const &id, Window const &win, nlohmann::json const &j)
    {
        if (j.contains("sprite"))
        {
            PROFILE_FUNCTION();

            if (std::string_view const &file{j["sprite"]["file"].get<std::string_view>()};
                j["sprite"].contains("crop"))
                add_object(id, {win, file, j["sprite"]["crop"]});
            else
                add_object(id, {win, file});
        }
    }

    void sprites_sys::to_json(object const &id, nlohmann::json &j)
    {
        PROFILE_FUNCTION();
        j["sprite"]["crop"] = {clip[id].x, clip[id].y, clip[id].w, clip[id].h};
    }

    void sprites_sys::draw_imgui(object const &id, Window const &win) noexcept {}

    Rectangle sprites_sys::draw_area(object const &id) const noexcept
    {
        PROFILE_FUNCTION();

        Rectangle ret;
        if (target != null_v<camera>)
        {
            Vector const &gPos{RotateVector(objects.gPos(id) - cameras.pos[target], -cameras.angle[target])};
            float const &dx{sprites.clip[id].w * objects.gScale(id).x * cameras.scale[target].x};
            float const &dy{sprites.clip[id].h * objects.gScale(id).y * cameras.scale[target].y};
            ret = {gPos.x, gPos.y, dx, dy};
        }
        else
        {
            float const &dx{clip[id].w * objects.gScale(id).x};
            float const &dy{clip[id].h * objects.gScale(id).y};
            ret = {objects.gPos(id).x, objects.gPos(id).y, dx, dy};
        }
        return ret;
    }

    void sprites_sys::target_cam(camera const &cam) noexcept
    {
        target = cam;
    }

    void sprites_sys::Draw(object const &id, Window const &win) const noexcept
    {
        if (active.contains(id))
        {
            PROFILE_FUNCTION();
            SDL_FRect const &dst{(SDL_FRect)draw_area(id)};

            SDL_RenderCopyExF(win.getRenderer(), tex[id],
                              &clip[id], &dst, objects.gAngle(id),
                              nullptr, SDL_FLIP_NONE);
        }
    }

    void sprites_sys::remove(object const &id) noexcept
    {
        PROFILE_FUNCTION();
        active.erase(id);
        tex.erase(tex.cbegin() + id);
        clip.erase(clip.cbegin() + id);
    }

    void sprites_sys::clear() noexcept
    {
        PROFILE_FUNCTION();
        active.clear();
        tex.clear();
        clip.clear();
    }
} // namespace tnt::doo
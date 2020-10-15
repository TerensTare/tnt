// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Cameras.hpp"
#include "doo_ecs/Objects.hpp"
#include "doo_ecs/Sprites.hpp"

#include "fileIO/AssetCache.hpp"
#include "json/JsonRectangle.hpp"

namespace tnt::doo
{
    inline medium_texture_cache *cache{default_texture_cache()};

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
        [[unlikely]] if (id > tex.capacity())
        {
            draw_queue.reserve(id - tex.capacity());
            tex.reserve(id - tex.capacity());
            clip.reserve(id - tex.capacity());
        }

        draw_queue.emplace(draw_queue.cbegin() + id, tex.size());
        tex.emplace(tex.cbegin() + id, sprite.tex);
        clip.emplace(clip.cbegin() + id, sprite.crop);
    }

    void sprites_sys::from_json(object const &id, Window const &win, nlohmann::json const &j)
    {
        if (json_has(j, "sprite"))
        {
            if (std::string_view const &file{j["sprite"]["file"].get<std::string_view>()};
                json_has(j["sprite"], "crop"))
                add_object(id, {win, file, j["sprite"]["crop"]});
            else
                add_object(id, {win, file});
        }
    }

    Rectangle sprites_sys::draw_area(object const &id) const noexcept
    {
        float const &dx{clip[id].w * objects.gScale(id).x};
        float const &dy{clip[id].h * objects.gScale(id).y};
        return {objects.gPos(id).x, objects.gPos(id).y, dx, dy};
    }

    inline const auto draw_area_cam =
        [](object const &id, tnt::doo::camera const &cam) noexcept -> SDL_FRect {
        Vector const &gPos{RotateVector(objects.gPos(id) - cameras.pos[cam], -cameras.angle[cam])};
        float const &dx{sprites.clip[id].w * objects.gScale(id).x * cameras.scale[cam].x};
        float const &dy{sprites.clip[id].h * objects.gScale(id).y * cameras.scale[cam].y};
        return SDL_FRect{gPos.x, gPos.y, dx, dy};
    };

    void sprites_sys::Draw(object const &id, Window const &win, tnt::doo::camera const &cam) const noexcept
    {
        if (draw_queue.size() <= id || draw_queue[id] != id)
            return;

        if (cam == null || cameras.active.size() <= cam)
        {
            SDL_FRect const &dst{(SDL_FRect)draw_area(id)};

            SDL_RenderCopyExF(win.getRenderer(), tex[id],
                              &clip[id], &dst,
                              objects.gAngle(id),
                              nullptr, SDL_FLIP_NONE);
        }
        else
            SDL_RenderCopyExF(win.getRenderer(), tex[id],
                              &clip[id], &draw_area_cam(id, cam),
                              objects.gAngle(id) - cameras.angle[cam],
                              nullptr, SDL_FLIP_NONE);
    }
} // namespace tnt::doo
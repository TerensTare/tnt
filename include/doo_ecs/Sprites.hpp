#ifndef TNT_DOO_ECS_SPRITES_HPP
#define TNT_DOO_ECS_SPRITES_HPP

#include "core/Window.hpp"
#include "doo_ecs/Base.hpp"
#include "fileIO/AssetCache.hpp"
#include "json/JsonRectangle.hpp"

// TODO: find a way to store the medium_texture_cache* on sprites_sys.

namespace tnt::doo
{
    struct sprite_comp final
    {
        inline sprite_comp(Window const &win, std::string_view filename) noexcept
            : crop{0, 0, 0, 0}
        {
            tex = cache->get(win.getRenderer(), filename);
            SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
            crop.w = w;
            crop.h = h;
        }

        inline sprite_comp(
            Window const &win, std::string_view filename,
            Rectangle const &rect) noexcept
            : crop{(int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h},
              w{(int)rect.w}, h{(int)rect.h}
        {
            tex = cache->get(win.getRenderer(), filename);
        }

        SDL_Texture *tex;
        SDL_Rect crop;

    private:
        int w, h;
        static medium_texture_cache *cache;
    };

    medium_texture_cache *sprite_comp::cache{default_texture_cache()};

    class sprites_sys final
    {
    public:
        inline void add_object(Window const &win, std::string_view filename)
        {
            const object index{sprite.size()};
            [[unlikely]] if (index == sprite.capacity())
            {
                sprite.reserve(5);
                draw_queue.reserve(5);
            }

            draw_queue.emplace_back(index);
            sprite.emplace_back(win, filename);
        }

        inline void add_object(Window const &win,
                               std::string_view filename,
                               Rectangle const &rect)
        {
            const object index{sprite.size()};
            [[unlikely]] if (index == sprite.capacity())
            {
                sprite.reserve(5);
                draw_queue.reserve(5);
            }

            draw_queue.emplace_back(index);
            sprite.emplace_back(win, filename, rect);
        }

        inline void from_json(Window const &win, nlohmann::json const &j)
        {
            std::string_view const file{j.at("sprite").at("file").get<std::string_view>()};
            if (j.at("sprite").at("crop").is_null())
                add_object(win, file);
            else
            {
                Rectangle const rect{j.at("sprite").at("crop")};
                add_object(win, file, rect);
            }
        }

        std::vector<object> draw_queue;
        std::vector<sprite_comp> sprite;
    } sprites;
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_SPRITES_HPP
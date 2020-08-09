#ifndef TNT_DOO_ECS_SPRITES_HPP
#define TNT_DOO_ECS_SPRITES_HPP

#include "core/Window.hpp"
#include "doo_ecs/Base.hpp"
#include "fileIO/AssetCache.hpp"
#include "json/JsonRectangle.hpp"

// TODO: find a way to store the medium_texture_cache* on sprites_sys.

namespace tnt::doo
{
    /// @brief A struct that handles an object's sprite data.
    struct sprite_comp final
    {
        /// @brief Create a new sprite component from an image file.
        /// @param win The window where the sprite will be drawed.
        /// @param file The name of the image file to load.
        inline sprite_comp(Window const &win, std::string_view filename) noexcept
            : crop{0, 0, 0, 0}
        {
            tex = cache->get(win.getRenderer(), filename);
            SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
            crop.w = w;
            crop.h = h;
        }

        /// @brief Create a new sprite component from an image file.
        /// @param win The window where the sprite will be drawed.
        /// @param file The name of the image file to load.
        /// @param rect The area of the image you want to have on the sprite.
        inline sprite_comp(
            Window const &win, std::string_view filename,
            Rectangle const &rect) noexcept
            : crop{(int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h},
              w{(int)rect.w}, h{(int)rect.h}
        {
            tex = cache->get(win.getRenderer(), filename);
        }

        SDL_Texture *tex; /// < The texture handle for the object.
        SDL_Rect crop;    /// < The area of the image represented on the sprite.

    private:
        int w, h;
        inline static medium_texture_cache *cache{default_texture_cache()};
    };

    /// @brief A struct that handles the sprite data for all the objects.
    inline struct sprites_sys final
    {
        /// @brief Add a new object to the sprites system.
        /// @param win The window where the object will be drawed.
        /// @param filename The name of the image file to load.
        inline void add_object(Window const &win, std::string_view filename)
        {
            object const index{tex.size()};
            [[unlikely]] if (index == tex.capacity())
            {
                draw_queue.reserve(5);
                tex.reserve(5);
                clip.reserve(5);
            }

            sprite_comp const &comp{win, filename};

            draw_queue.emplace_back(index);
            tex.emplace_back(comp.tex);
            clip.emplace_back(comp.crop);
        }

        /// @brief Add a new object to the sprites system.
        /// @param win The window where the object will be drawed.
        /// @param filename The name of the image file to load.
        /// @param rect The area of the image you want to have on the sprite.
        inline void add_object(Window const &win,
                               std::string_view filename,
                               Rectangle const &rect)
        {
            object const index{tex.size()};
            [[unlikely]] if (index == tex.capacity())
            {
                draw_queue.reserve(5);
                tex.reserve(5);
                clip.reserve(5);
            }

            sprite_comp const &comp{win, filename, rect};

            draw_queue.emplace_back(index);
            tex.emplace_back(comp.tex);
            clip.emplace_back(comp.crop);
        }

        /// @brief Load the sprite data of the objects from a given json chunk.
        /// @param j The json chunk from where to load the sprite data of the objects.
        inline void from_json(Window const &win, nlohmann::json const &j)
        {
            std::string_view const file{j["sprite"]["file"].get<std::string_view>()};
            if (j["sprite"]["crop"].is_null())
                add_object(win, file);
            else
            {
                Rectangle const rect{j["sprite"]["crop"]};
                add_object(win, file, rect);
            }
        }

        /// @brief Draw object with the given id on the given window.
        /// @param id The id of the object to draw.
        /// @param win The window where the object will be drawed.
        inline void Draw(object id, Window const &win) noexcept
        {
            float const dx{sprites.clip[id].w * objects.scale[id].x * .5f};
            float const dy{sprites.clip[id].h * objects.scale[id].y * .5f};
            SDL_FRect const dst{objects.pos[id].x - dx, objects.pos[id].y - dy, 2 * dx, 2 * dy};

            SDL_RenderCopyExF(
                win.getRenderer(), sprites.tex[id],
                &sprites.clip[id], &dst, objects.angle[id],
                nullptr, SDL_FLIP_NONE);
        }

        std::vector<object> draw_queue; /// < All the id-s of the objects that should be drawed.
        std::vector<SDL_Texture *> tex; /// < The texture data of the objects.
        std::vector<SDL_Rect> clip;     /// < The cropped area from the image.
    } sprites;                          /// < An instance of sprites_sys.
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_SPRITES_HPP
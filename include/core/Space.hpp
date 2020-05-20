#ifndef TNT_SPACE_HPP
#define TNT_SPACE_HPP

#include <map>
#include <string>

#include "core/Camera.hpp"
#include "core/Window.hpp"

// a utility to group several objects, similar to a layer.

// TODO: getObject should have conditional noexcept (if key exists in
// container).
// TODO: implement Draw().

// TODO(maybe):
// Add a Quadtree ??

namespace tnt
{
    class Object;
    class Window;

    namespace detail
    {
        auto should_draw = [](tnt::Rectangle const &bounds, tnt::Vector const &pos, float w, float h) -> bool {
            return (bounds.Contains(pos) ||
                    bounds.Contains(pos + tnt::Vector{w, 0}) ||
                    bounds.Contains(pos + tnt::Vector{0, h}) ||
                    bounds.Contains(pos + tnt::Vector{w, h}));
        };
    } // namespace detail

    class Space
    {
    public:
        void addObject(std::pair<std::string_view, Object *> const &obj);
        void addObject(std::string_view id, Object *obj);

        Object *getObject(std::string_view id) const;
        void removeObject(std::string_view id) noexcept;

        template <camera C>
        void Draw(Window const *win, C const &cam) noexcept
        {
            for (auto const &it : objects)
                if (it.second->isActive() &&
                    it.second->has<SpriteComponent>())
                {
                    Sprite *spr{dynamic_cast<Sprite *>(it.second)};
                    SpriteComponent *sprite{spr->getSprite()};

                    const int w{sprite->getWidth()};
                    const int h{sprite->getHeight()};

                    if (tnt::Vector pos{spr->getPosition()};
                        detail::should_draw(cam.Bounds(), pos, static_cast<float>(w), static_cast<float>(h)))
                        win->Draw(spr,
                                  SDL_Rect{static_cast<int>(pos.x), static_cast<int>(pos.y), w, h}, cam);
                }
        }

        void Update(long long time_) noexcept;

    protected:
        std::map<std::string, Object *> objects;
    };
} // namespace tnt

#endif //!TNT_SPACE_HPP
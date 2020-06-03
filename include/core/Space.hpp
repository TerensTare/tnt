#ifndef TNT_SPACE_HPP
#define TNT_SPACE_HPP

#include <map>
#include <string>

#include "core/Camera.hpp"
#include "core/Window.hpp"

// a utility to group several objects, similar to a layer.

// TODO: getObject should have conditional noexcept (if key exists in
// container).
// TODO: detect collision at Update().
// TODO: make Object-s inactive if out of the camera.
// TODO: find way to reduce for loops.

// TODO(maybe):
// Add a Quadtree ??
// tnt::Space::operator[] ??

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
        bool isActive() const noexcept;

        void addObject(std::pair<std::string_view, Object *> const &obj);
        void addObject(std::string_view id, Object *obj);

        Object *getObject(std::string_view id) const;
        void removeObject(std::string_view id) noexcept;

        template <camera C>
        inline void Draw(Window const *win, C const &cam) noexcept
        {
            for (auto const &it : objects)
                if (it.second->has<SpriteComponent>())
                {
                    SpriteComponent *sprite{it.second->get<SpriteComponent>()};

                    const int w{sprite->getWidth()};
                    const int h{sprite->getHeight()};

                    if (tnt::Vector const pos{it.second->getPosition()};
                        detail::should_draw(cam.Bounds(), pos, static_cast<float>(w), static_cast<float>(h)))
                    {
                        it.second->setActive(true);

                        Vector const scale{it.second->getScale()};
                        Rectangle const dest{
                            pos.x - sprite->getWidth() * scale.x * .5f,
                            pos.y - sprite->getHeight() * scale.y * .5f,
                            sprite->getWidth() * scale.x, sprite->getHeight() * scale.y};

                        sprite->Draw(win, dest, it.second->getAngle());
                    }
                    else
                        it.second->setActive(false);
                }
        }

        void Update(long long time_) noexcept;

    protected:
        bool active{true};
        std::map<std::string, Object *> objects;
    }; // namespace tnt
} // namespace tnt

#endif //!TNT_SPACE_HPP
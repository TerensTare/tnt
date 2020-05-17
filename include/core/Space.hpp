#ifndef TNT_SPACE_HPP
#define TNT_SPACE_HPP

#include <map>
#include <string>
#include "core/Camera.hpp"

// a utility to group several objects, similar to a layer.

// TODO: getObject should have conditional noexcept (if key exists in
// container).
// TODO: implement Draw().

// TODO(maybe):
// Add a Quadtree.
// Use a map to save Object id-s.

namespace tnt
{
    class Object;

    class Space
    {
    public:
        ~Space() noexcept;

        void addObject(std::pair<std::string_view, Object *> const &obj);
        void addObject(std::string_view id, Object *obj);

        Object *getObject(std::string_view id) const;
        void removeObject(std::string_view id) noexcept;

        // TODO: unimplemented
        void Draw(Window const *win, camera auto const &cam);
        void Update(long long time_);

    protected:
        std::map<std::string, Object *> objects;
    };
} // namespace tnt

#endif //! TNT_SPACE_HPP
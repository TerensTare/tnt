#ifndef TNT_SPACE_HPP
#define TNT_SPACE_HPP

#include <string>
#include "ecs/Object.hpp"

// a utility to group several objects, similar to a layer.

// TODO: getObject should have conditional noexcept (if key exists in container).

// TODO(maybe):
// Add a Quadtree.
// Use a map to save Object id-s.

namespace tnt
{
class Space
{
public:
    std::string getId() const noexcept;
    void setId(std::string_view name) noexcept;

    void addObject(Object &&obj) noexcept(noexcept(objects.push_back));
    Object &getObject() const noexcept(true);

private:
    std::string id;
    std::vector<Object *> objects;
};
} // namespace tnt

#endif //!TNT_SPACE_HPP
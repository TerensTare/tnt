#ifndef TNT_SPACE_HPP
#define TNT_SPACE_HPP

#include <map>
#include <string>

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
class Camera;

class Space
{
public:
    ~Space() noexcept;

    void addObject(std::pair<std::string_view, Object *> const &obj);
    void addObject(std::string_view id, Object *obj) noexcept;

    Object *getObject(std::string_view id) const;
    void removeObject(std::string_view id) noexcept;

    // TODO: unimplemented
    void Draw(Camera const *cam);
    void Update(long long time_);

protected:
    std::map<std::string, Object *> objects;
};
} // namespace tnt

#endif //! TNT_SPACE_HPP
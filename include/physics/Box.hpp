#ifndef BOX_HPP
#define BOX_HPP

#include "physics/Vector.hpp"

// TODO: make sure T is a math type or a built-in non-char type.
// TODO: make sure w and h are positive.

namespace tnt
{
template <typename T>
struct Box
{
    constexpr Box(T l = 0, T t = 0, T w = 0, T h = 0) noexcept
        : left{l}, top{t}, w{w}, h{h} {}

    constexpr Box(Vector<T> const &pos, Vector<T> const &size) noexcept
        : left{pos.x}, top{pos.y}, width{size.x}, height{size.y} {}

    constexpr T right() const noexcept
    {
        return left + width;
    }

    constexpr T bottom() const noexcept
    {
        return top + height;
    }

    constexpr Vector<T> topleft() const noexcept
    {
        return Vector<T>{left, top};
    }

    constexpr Vector<T> center() const noexcept
    {
        return Vector<T>{(left + width) / 2, (top + height) / 2};
    }

    constexpr Vector<T> size() const noexcept
    {
        return Vector<T>{width, height};
    }

    constexpr bool contains(Box<T> const &box) const noexcept
    {
        return ((left <= box.left) && (box.right() <= right()) &&
                (top <= box.top) && (box.bottom() <= bottom()));
    }

    constexpr bool intersects(Box<T> const &box) const noexcept
    {
        return !((left >= box.right()) || (right() <= box.left) ||
                 (top >= box.bottom()) || (bottom() <= box.top));
    }

    T left;
    T top;
    T width;
    T height;
};

template <>
struct Box<void>
{
    Box(void) {}
};
} // namespace tnt

#endif //!BOX_HPP
#ifndef TNT_RECTANGLE_HPP
#define TNT_RECTANGLE_HPP

#include "math/Vector.hpp"

struct SDL_Rect;
typedef struct SDL_Rect SDL_Rect;

struct SDL_FRect;
typedef struct SDL_FRect SDL_FRect;

namespace tnt
{
// TODO: WIP
// TODO: different operators support.

// TODO(maybe):
// conversion operators for SDL_Rect and SDL_FRect ??
// add constexpr on all functions ??
struct Rectangle
{
    constexpr Rectangle(float x_, float y_, float w_, float h_) noexcept
        : x{x_}, y{y_}, w{w_}, h{h_} {}

    constexpr Rectangle(int x_, int y_, int w_, int h_) noexcept
        : x{static_cast<float>(x_)}, y{static_cast<float>(y_)},
          w{static_cast<float>(w_)}, h{static_cast<float>(h_)} {}

    constexpr Rectangle(Vector const &pos, float w_, float h_) noexcept
        : x{pos.x}, y{pos.y}, w{w_}, h{h_} {}

    Rectangle(Rectangle const &) = default; // momentally
    Rectangle(Rectangle &&) = default;      // momentally

    Rectangle &operator=(Rectangle const &other) noexcept
    {
        x = other.x;
        y = other.y;
        w = other.w;
        h = other.h;
        return *this;
    }

    Rectangle &operator=(Rectangle &&other) noexcept = default;

    Rectangle &operator+=(Rectangle const &other) noexcept
    {
        x = x + other.x;
        y = y + other.y;
        w = w + other.w;
        h = h + other.h;
    }

    Rectangle &operator+=(Vector const &pos) noexcept
    {
        x = x + pos.x;
        y = y + pos.y;
    }

    Rectangle &operator-=(Rectangle const &other) noexcept
    {
        x = x - other.x;
        y = y - other.y;
        w = w - other.w;
        h = h - other.h;
    }

    Rectangle &operator-=(Vector const &pos) noexcept
    {
        x = x - pos.x;
        y = y - pos.y;
    }

    Rectangle &operator*=(float const &scale) noexcept
    {
        w = w * scale;
        h = h * scale;
    }

    Rectangle &operator*=(Vector const &scale) noexcept
    {
        w = w * scale.x;
        h = h * scale.y;
    }

    Rectangle &operator/=(float const &scale) noexcept
    {
        w = w / scale;
        h = h / scale;
    }

    Rectangle &operator/=(Vector const &scale) noexcept
    {
        w = w / scale.x;
        h = h / scale.y;
    }

    operator SDL_Rect() noexcept;
    operator SDL_FRect() noexcept;

    friend std::ostream &operator<<(std::ostream &os, Rectangle const &rhs);

    inline bool Outside(Vector const &point) const noexcept
    {
        return ((point.x < x) || (point.x > x + w) || (point.y < y) || (point.y > y + h));
    }

    inline bool Contains(Vector const &point) const noexcept
    {
        return !Outside(point);
    }

    float x;
    float y;
    float w;
    float h;
};

inline Rectangle operator+(Rectangle const &lhs, Rectangle const &rhs) noexcept
{
    return Rectangle{lhs.x + rhs.x, lhs.y + rhs.y,
                     lhs.w + rhs.w, lhs.h + rhs.h};
}

inline Rectangle operator-(Rectangle const &lhs, Rectangle const &rhs) noexcept
{
    return Rectangle{lhs.x - rhs.x, lhs.y - rhs.y,
                     lhs.w - rhs.w, lhs.h - rhs.h};
}

inline Rectangle operator*(Rectangle const &lhs, float const &num) noexcept
{
    return Rectangle{lhs.x, lhs.y, lhs.w * num, lhs.h * num};
}

inline Rectangle operator/(Rectangle const &lhs, float const &num) noexcept
{
    return Rectangle{lhs.x, lhs.y, lhs.w / num, lhs.h / num};
}

inline Rectangle operator*(Rectangle const &lhs, Vector const &scale) noexcept
{
    return Rectangle{lhs.x, lhs.y, lhs.w * scale.x, lhs.h * scale.y};
}

inline Rectangle operator/(Rectangle const &lhs, Vector const &scale) noexcept
{
    return Rectangle{lhs.x, lhs.y, lhs.w / scale.x, lhs.h / scale.y};
}

inline bool operator==(Rectangle const &lhs, Rectangle const &rhs) noexcept
{
    return ((lhs.x == rhs.x) && (lhs.y == rhs.y) &&
            (lhs.w == rhs.w) && (lhs.h == rhs.h));
}

inline std::ostream &tnt::operator<<(std::ostream &os, tnt::Rectangle const &rhs)
{
    os << "{" << rhs.x << ", " << rhs.y << ", "
       << rhs.w << ", " << rhs.h << "}";
    return os;
}
} // namespace tnt

#endif //!TNT_RECTANGLE_HPP
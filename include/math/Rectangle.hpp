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
    struct Rectangle
    {
        // Needed for JSON
        inline constexpr Rectangle() noexcept
            : x{0.f}, y{0.f}, w{0.f}, h{0.f} {}

        inline constexpr Rectangle(float x_, float y_, float w_, float h_) noexcept
            : x{x_}, y{y_}, w{w_}, h{h_} {}

        inline constexpr Rectangle(int x_, int y_, int w_, int h_) noexcept
            : x{static_cast<float>(x_)}, y{static_cast<float>(y_)},
              w{static_cast<float>(w_)}, h{static_cast<float>(h_)} {}

        inline constexpr Rectangle(Vector const &pos, float w_, float h_) noexcept
            : x{pos.x}, y{pos.y}, w{w_}, h{h_} {}

        Rectangle(Rectangle const &) = default;
        Rectangle(Rectangle &&) = default;

        inline constexpr Rectangle &operator=(Rectangle const &other) noexcept
        {
            x = other.x;
            y = other.y;
            w = other.w;
            h = other.h;
            return *this;
        }

        Rectangle &operator=(Rectangle &&other) noexcept = default;

        inline constexpr Rectangle &operator+=(Rectangle const &other) noexcept
        {
            x += other.x;
            y += other.y;
            w += other.w;
            h += other.h;
            return *this;
        }

        inline constexpr Rectangle &operator+=(Vector const &pos) noexcept
        {
            x += pos.x;
            y += pos.y;
            return *this;
        }

        inline constexpr Rectangle &operator-=(Rectangle const &other) noexcept
        {
            x -= other.x;
            y -= other.y;
            w -= other.w;
            h -= other.h;
            return *this;
        }

        inline constexpr Rectangle &operator-=(Vector const &pos) noexcept
        {
            x -= pos.x;
            y -= pos.y;
            return *this;
        }

        inline constexpr Rectangle &operator*=(float const &scale) noexcept
        {
            w *= scale;
            h *= scale;
            return *this;
        }

        inline constexpr Rectangle &operator*=(Vector const &scale) noexcept
        {
            w *= scale.x;
            h *= scale.y;
            return *this;
        }

        inline constexpr Rectangle &operator/=(float const &scale) noexcept
        {
            w /= scale;
            h /= scale;
            return *this;
        }

        inline constexpr Rectangle &operator/=(Vector const &scale) noexcept
        {
            w /= scale.x;
            h /= scale.y;
            return *this;
        }

        operator SDL_Rect() noexcept;
        operator SDL_FRect() noexcept;

        operator const SDL_Rect() const noexcept;
        operator const SDL_FRect() const noexcept;

        inline constexpr bool Outside(Vector const &point) const noexcept
        {
            return ((point.x < x) || (point.x > x + w) || (point.y < y) ||
                    (point.y > y + h));
        }

        inline constexpr bool Contains(Vector const &point) const noexcept
        {
            return !Outside(point);
        }

        float x;
        float y;
        float w;
        float h;
    };

    inline constexpr Rectangle operator+(Rectangle const &lhs,
                                         Rectangle const &rhs) noexcept
    {
        return Rectangle{lhs.x + rhs.x, lhs.y + rhs.y, lhs.w + rhs.w,
                         lhs.h + rhs.h};
    }

    inline constexpr Rectangle operator+(Rectangle const &lhs,
                                         Vector const &rhs) noexcept
    {
        return Rectangle{lhs.x + rhs.x, lhs.y + rhs.y,
                         lhs.w, lhs.h};
    }

    inline constexpr Rectangle operator-(Rectangle const &lhs,
                                         Rectangle const &rhs) noexcept
    {
        return Rectangle{lhs.x - rhs.x, lhs.y - rhs.y, lhs.w - rhs.w,
                         lhs.h - rhs.h};
    }

    inline constexpr Rectangle operator-(Rectangle const &lhs,
                                         Vector const &rhs) noexcept
    {
        return Rectangle{lhs.x - rhs.x, lhs.y - rhs.y,
                         lhs.w, lhs.h};
    }

    inline constexpr Rectangle operator*(Rectangle const &lhs, float const &num) noexcept
    {
        return Rectangle{lhs.x, lhs.y, lhs.w * num, lhs.h * num};
    }

    inline constexpr Rectangle operator/(Rectangle const &lhs, float const &num) noexcept
    {
        return Rectangle{lhs.x, lhs.y, lhs.w / num, lhs.h / num};
    }

    inline constexpr Rectangle operator*(Rectangle const &lhs,
                                         Vector const &scale) noexcept
    {
        return Rectangle{lhs.x, lhs.y, lhs.w * scale.x, lhs.h * scale.y};
    }

    inline constexpr Rectangle operator/(Rectangle const &lhs,
                                         Vector const &scale) noexcept
    {
        return Rectangle{lhs.x, lhs.y, lhs.w / scale.x, lhs.h / scale.y};
    }

    inline bool operator==(Rectangle const &lhs, Rectangle const &rhs) noexcept
    {
        return ((fabs(lhs.x - rhs.x) < FLT_EPSILON) &&
                (fabs(lhs.y - rhs.y) < FLT_EPSILON) &&
                (fabs(lhs.w - rhs.w) < FLT_EPSILON) &&
                (fabs(lhs.h - rhs.h) < FLT_EPSILON));
    }

    inline std::ostream &operator<<(std::ostream &os,
                                    tnt::Rectangle const &rhs)
    {
        os << "{" << rhs.x << ", " << rhs.y << ", " << rhs.w << ", " << rhs.h
           << "}";
        return os;
    }
} // namespace tnt

#endif //! TNT_RECTANGLE_HPP
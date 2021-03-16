#ifndef TNT_RECTANGLE_HPP
#define TNT_RECTANGLE_HPP

#include "core/Config.hpp"
#include "math/Vector.hpp"

struct SDL_Rect;
typedef struct SDL_Rect SDL_Rect;

struct SDL_FRect;
typedef struct SDL_FRect SDL_FRect;

namespace tnt
{
    struct TNT_API Rectangle
    {
        // Needed for JSON
        constexpr Rectangle() noexcept
            : x{0.f}, y{0.f}, w{0.f}, h{0.f} {}

        constexpr Rectangle(float x_, float y_, float w_, float h_) noexcept
            : x{x_}, y{y_}, w{w_}, h{h_} {}

        constexpr Rectangle(int x_, int y_, int w_, int h_) noexcept
            : x{static_cast<float>(x_)}, y{static_cast<float>(y_)},
              w{static_cast<float>(w_)}, h{static_cast<float>(h_)} {}

        constexpr Rectangle(Vector const &pos, float w_, float h_) noexcept
            : x{pos.x}, y{pos.y}, w{w_}, h{h_} {}

        Rectangle(Rectangle const &) = default;
        Rectangle(Rectangle &&) = default;

        constexpr Rectangle &operator=(Rectangle const &other) noexcept
        {
            x = other.x;
            y = other.y;
            w = other.w;
            h = other.h;
            return *this;
        }

        Rectangle &operator=(Rectangle &&other) noexcept = default;

        constexpr Rectangle &operator+=(Rectangle const &other) noexcept
        {
            x += other.x;
            y += other.y;
            w += other.w;
            h += other.h;
            return *this;
        }

        constexpr Rectangle &operator+=(Vector const &pos) noexcept
        {
            x += pos.x;
            y += pos.y;
            return *this;
        }

        constexpr Rectangle &operator-=(Rectangle const &other) noexcept
        {
            x -= other.x;
            y -= other.y;
            w -= other.w;
            h -= other.h;
            return *this;
        }

        constexpr Rectangle &operator-=(Vector const &pos) noexcept
        {
            x -= pos.x;
            y -= pos.y;
            return *this;
        }

        constexpr Rectangle &operator*=(float const &scale) noexcept
        {
            w *= scale;
            h *= scale;
            return *this;
        }

        constexpr Rectangle &operator*=(Vector const &scale) noexcept
        {
            w *= scale.x;
            h *= scale.y;
            return *this;
        }

        constexpr Rectangle &operator/=(float const &scale) noexcept
        {
            w /= scale;
            h /= scale;
            return *this;
        }

        constexpr Rectangle &operator/=(Vector const &scale) noexcept
        {
            w /= scale.x;
            h /= scale.y;
            return *this;
        }

        operator SDL_Rect() noexcept;
        operator SDL_FRect() noexcept;

        operator const SDL_Rect() const noexcept;
        operator const SDL_FRect() const noexcept;

        constexpr bool Outside(Vector const &point) const noexcept
        {
            return !Contains(point);
        }

        constexpr bool Contains(Vector const &point) const noexcept
        {
            return ((point.x > x) && (point.x < x + w) && (point.y > y) &&
                    (point.y < y + h));
        }

        constexpr Vector TopLeft() const noexcept { return Vector{x, y}; }
        constexpr Vector TopRight() const noexcept { return Vector{x + w, y}; }
        constexpr Vector BottomLeft() const noexcept { return Vector{x, y + h}; }
        constexpr Vector BottomRight() const noexcept { return Vector{x + w, y + h}; }

        float x;
        float y;
        float w;
        float h;
    };

    constexpr Rectangle operator+(Rectangle const &lhs,
                                  Rectangle const &rhs) noexcept
    {
        return Rectangle{lhs.x + rhs.x, lhs.y + rhs.y, lhs.w + rhs.w,
                         lhs.h + rhs.h};
    }

    constexpr Rectangle operator+(Rectangle const &lhs,
                                  Vector const &rhs) noexcept
    {
        return Rectangle{lhs.x + rhs.x, lhs.y + rhs.y,
                         lhs.w, lhs.h};
    }

    constexpr Rectangle operator-(Rectangle const &lhs,
                                  Rectangle const &rhs) noexcept
    {
        return Rectangle{lhs.x - rhs.x, lhs.y - rhs.y, lhs.w - rhs.w,
                         lhs.h - rhs.h};
    }

    constexpr Rectangle operator-(Rectangle const &lhs,
                                  Vector const &rhs) noexcept
    {
        return Rectangle{lhs.x - rhs.x, lhs.y - rhs.y,
                         lhs.w, lhs.h};
    }

    constexpr Rectangle operator*(Rectangle const &lhs, float const &num) noexcept
    {
        return Rectangle{lhs.x, lhs.y, lhs.w * num, lhs.h * num};
    }

    constexpr Rectangle operator/(Rectangle const &lhs, float const &num) noexcept
    {
        return Rectangle{lhs.x, lhs.y, lhs.w / num, lhs.h / num};
    }

    constexpr Rectangle operator*(Rectangle const &lhs,
                                  Vector const &scale) noexcept
    {
        return Rectangle{lhs.x, lhs.y, lhs.w * scale.x, lhs.h * scale.y};
    }

    constexpr Rectangle operator/(Rectangle const &lhs,
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

    inline std::ostream &operator<<(std::ostream &os_,
                                    tnt::Rectangle const &rhs)
    {
        os_ << "{" << rhs.x << ", " << rhs.y << ", " << rhs.w << ", " << rhs.h
           << "}";
        return os_;
    }
} // namespace tnt

#endif //! TNT_RECTANGLE_HPP
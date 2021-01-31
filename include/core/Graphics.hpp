#ifndef TNT_CORE_GRAPHICS_HELPER_API_HPP
#define TNT_CORE_GRAPHICS_HELPER_API_HPP

#include <span>

#include "core/Window.hpp"
#include "math/Rectangle.hpp"

// TODO:
// load this to Lua
// more shapes
// general polygons

// TODO(maybe):
// custom thickness for circles ??

namespace tnt
{
    /// @brief Draw a line on the given window with the specified @a color from @a start to @a finish that is @a thickness pixels thick.
    /// @param win The window where the line will be drawed.
    /// @param start The starting point of the line.
    /// @param finish The other tip of the line.
    /// @param color The color that will be used to draw the line. Default to white.
    /// @param thickness Parameter specifying how thick the line will be. Default to 1.
    TNT_API void drawLine(Window const &win, Vector const &start, Vector const &finish,
                          SDL_Color const &color = SDL_Color{255, 255, 255}, unsigned const thickness = 1) noexcept;

    /// @brief Draw a horizontal line on the given window with the specified @a color from ( @a x1, @a y) to ( @a x2, @a y) that is @a thickness pixels thick.
    /// @param win The window where the line will be drawed.
    /// @param x1 The starting x position of the line.
    /// @param x2 The x position of the other tip of the line.
    /// @param y The y position of each point on the line.
    /// @param color The color that will be used to draw the line. Default to white.
    /// @param thickness Parameter specifying how thick the line will be. Default to 1.
    TNT_API void drawHorizontalLine(Window const &win, float const x1, float const x2, float const y,
                                    SDL_Color const &color = SDL_Color{255, 255, 255}, unsigned const thickness = 1) noexcept;

    /// @brief Draw a vertical line on the given window with the specified @a color from ( @a x, @a y1) to ( @a x, @a y2) that is @a thickness pixels thick.
    /// @param win The window where the line will be drawed.
    /// @param x The x position of each point on the line.
    /// @param y1 The starting y position of the line.
    /// @param y2 The y position of the other tip of the line.
    /// @param color The color that will be used to draw the line. Default to white.
    /// @param thickness Parameter specifying how thick the line will be. Default to 1.
    TNT_API void drawVerticalLine(Window const &win, float const x, float const y1, float const y2,
                                  SDL_Color const &color = SDL_Color{255, 255, 255}, unsigned const thickness = 1) noexcept;

    /// @brief Draw lines defined by @a points with the given @a color on @a win.
    /// @param win The window where the lines will be drawed.
    /// @param points The points that define the lines borders. points.size() should be even.
    /// @param color The color that will be used to draw the lines.
    TNT_API void drawLines(Window const &win, std::span<tnt::Vector> points,
                           SDL_Color const &color = SDL_Color{255, 255, 255}) noexcept;

    /// @brief Draw an empty circle with the given @a radius and @a color on @a win.
    /// @param win The window where the circle will be drawed.
    /// @param topleft The leftmost and topmost point.
    /// @param radius The radius of the circle.
    /// @param color The color of the circle.
    /// @param thickness The thickness of the border of the circle.
    TNT_API void drawCircle(Window const &win, Vector const &topleft,
                            float const radius, SDL_Color const &color = SDL_Color{255, 255, 255},
                            unsigned const thickness = 1) noexcept;

    /// @brief Draw a filled circle with the given @a radius and @a color on @a win.
    /// @param win The window where the circle will be drawed.
    /// @param topleft The leftmost and topmost point.
    /// @param radius The radius of the circle.
    /// @param color The color of the circle.
    TNT_API void fillCircle(Window const &win, Vector const &topleft,
                            float const radius, SDL_Color const &color = SDL_Color{255, 255, 255}) noexcept;
} // namespace tnt

#endif //!TNT_CORE_GRAPHICS_HELPER_API_HPP
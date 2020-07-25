#ifndef TNT_DOO_ECS_ANIMATED_OBJECTS_HPP
#define TNT_DOO_ECS_ANIMATED_OBJECTS_HPP

#include "doo_ecs/Sprites.hpp"
#include "json/JsonRectangle.hpp"
#include "utils/TypeUtils.hpp"

// TODO:
// timed animations (on wrap_mode)
// TODO(maybe):
// hide some members ??
// remove finished/passive members on each system ?? (you don't need to iterate them)

namespace tnt::doo
{
    /// @brief A struct that holds the data for an animation.
    struct animation_comp final
    {
        /// @brief The wrap mode of the animation.
        enum wrap_mode : unsigned
        {
            single_run = 0, /// < Run the animation only once.
            loop            /// < Run the animation infinitely.
        };

        /// @brief The direction the frames are arranged on the animation image.
        enum direction : unsigned
        {
            horizontal = 0, /// < The frames are arranged horizontally.
            vertical        /// < The frames are arranged vertically.
        };

        /// @brief Create a new animation component.
        /// @param rect The size of a frame on the image.
        /// @param framesCount The number of frames that the animation has.
        /// @param animSpeed The speed the animation should be played with.
        /// @param space The spacing between the frames of the sprites, 0.f if none.
        /// @param dir_ The direction of the frames on the image.
        /// @param wrap_ The wrap mode of the animation.
        inline constexpr animation_comp(Rectangle const &rect,
                                        int const framesCount, float const animSpeed, float const space,
                                        direction const &dir_, wrap_mode const &wrap_) noexcept
            : clip{rect}, startX{rect.x}, startY{rect.y},
              speed{animSpeed}, timePerFrame{animSpeed / framesCount},
              spacing{space}, dir{dir_}, wrap{wrap_} {}

        bool finished{false};
        wrap_mode wrap;
        direction dir;
        float startX, startY;
        float speed, timePerFrame;
        float elapsed{0.f};
        float spacing;
        Rectangle clip;
    };

    /// @brief The system that controls the animations of the sprites.
    inline struct animations_sys final
    {
        /// @brief Add an object to the animations system.
        /// @param anim_ The data of the animation.
        inline void add_object(animation_comp const &anim_)
        {
            object const index{wrap.size()};
            [[unlikely]] if (index == wrap.capacity())
            {
                wrap.reserve(5);
                dir.reserve(5);
                startX.reserve(5);
                startY.reserve(5);
                elapsed.reserve(5);
                speed.reserve(5);
                timePerFrame.reserve(5);
                spacing.reserve(5);
                current.reserve(5);
                running.reserve(5);

                finished.reserve(5);
            }

            wrap.emplace_back(anim_.wrap);
            dir.emplace_back(anim_.dir);
            startX.emplace_back(anim_.startX);
            startY.emplace_back(anim_.startY);
            elapsed.emplace_back(0.f);
            speed.emplace_back(anim_.speed);
            timePerFrame.emplace_back(anim_.timePerFrame);
            spacing.emplace_back(anim_.spacing);
            current.emplace_back(0);

            running.emplace_back(if_else(!anim_.finished, index, -1)); // -1 or index
            finished.emplace_back(if_else(anim_.finished, index, -1)); // -1 or index
        }

        /// @brief Update the animation of the given object.
        /// @param id The id of the object's animation to update.
        /// @param time_ The time elapsed since the last Update() call.
        inline void Update(object id, float time_)
        {
            elapsed[id] += time_;
            if (elapsed[id] - (timePerFrame[id] * (current[id] + 1)) >= FLT_EPSILON)
            {
                ++current[id];
                int const index_{current[id] % int(speed[id] / timePerFrame[id])};
                if (wrap[id] == animation_comp::loop)
                {
                    sprites.clip[id].x = (int)if_then(
                        dir[id] == animation_comp::horizontal,
                        startX[id] + (sprites.clip[id].w + spacing[id]) * index_);
                    sprites.clip[id].y = (int)if_then(
                        dir[id] == animation_comp::vertical,
                        startY[id] + (sprites.clip[id].h + spacing[id]) * index_);
                }
                else
                {
                    running[id] = -1;
                    finished[id] = id;
                    elapsed[id] = speed[id] - timePerFrame[id];
                    sprites.clip[id].x = (int)startX[id];
                    sprites.clip[id].y = (int)startY[id];
                }
            }
        }

        /// @brief Load animation data from a json chunk.
        /// @param j The json chunk.
        inline void from_json(nlohmann::json const &j)
        {
            Rectangle const rect{j.at("sprite").at("crop")};
            nlohmann::json const &chunk{j["anim"]};
            int const frames{chunk["frames"]};
            float const speed{chunk["speed"]};
            float const space{
                if_else(chunk.at("space").is_null(),
                        0.f, chunk["space"])};
            animation_comp::direction const dir_{
                if_else(chunk["dir"] == "horizontal",
                        animation_comp::horizontal,
                        if_then(chunk["dir"] == "vertical",
                                animation_comp::vertical))};
            animation_comp::wrap_mode const wrap_{
                if_else(chunk["wrap"] == "once",
                        animation_comp::single_run,
                        if_then(chunk["wrap"] == "loop",
                                animation_comp::loop))};

            add_object(animation_comp{rect, frames, speed, space, dir_, wrap_});
        }

        std::vector<animation_comp::wrap_mode> wrap; /// < The wrap modes of the animations.
        std::vector<animation_comp::direction> dir;  /// < The direction of the frames on the image.
        std::vector<float> startX;                   /// < The starting x coordinate of the animation.
        std::vector<float> startY;                   /// < The starting y coordinate of the animation.
        std::vector<float> speed;                    /// < The speed of the animation.
        std::vector<float> elapsed;                  /// < The elapsed time since the beginning of the animation.
        std::vector<float> timePerFrame;             /// < Shows how long a frame should be drawed.
        std::vector<float> spacing;                  /// < The spacing between the frames of the sprites, 0 if none.
        std::vector<int> current;                    /// < The index of the current frame.

        std::vector<object> running;  /// < The id-s of the running animations.
        std::vector<object> finished; /// < The id-s of the finished animations.
    } animations;
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_ANIMATED_OBJECTS_HPP
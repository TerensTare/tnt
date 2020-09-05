#ifndef TNT_DOO_ECS_ANIMATED_OBJECTS_HPP
#define TNT_DOO_ECS_ANIMATED_OBJECTS_HPP

#include "doo_ecs/Base.hpp"
#include "math/Rectangle.hpp"

// TODO:
// timed animations (on wrap_mode)
// multiple animations for each object.
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
        constexpr animation_comp(Rectangle const &rect,
                                 int const framesCount, float const animSpeed, float const space,
                                 direction const &dir_, wrap_mode const &wrap_) noexcept;

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
        inline animations_sys() noexcept = default;

        animations_sys(animations_sys const &) = delete;
        animations_sys &operator=(animations_sys const &) = delete;

        /// @brief Add an object to the animations system.
        /// @param anim_ The data of the animation.
        void add_object(animation_comp const &anim_);

        /// @brief Add a new object with invalid data to the next index.
        /// Useful when you want the object with next id not to be in a certain system.
        void add_invalid();

        /// @brief Update the animation of the given object.
        /// @param id The id of the object's animation to update.
        /// @param time_ The time elapsed since the last Update() call.
        void Update(object const &id, float time_) noexcept;

        /// @brief Load animation data from a json chunk.
        /// @param j The json chunk.
        void from_json(nlohmann::json const &j);

        std::vector<animation_comp::wrap_mode> wrap; /// < The wrap modes of the animations.
        std::vector<animation_comp::direction> dir;  /// < The direction of the frames on the image.
        std::vector<float> startX;                   /// < The starting x coordinate of the animation.
        std::vector<float> startY;                   /// < The starting y coordinate of the animation.
        std::vector<float> speed;                    /// < The speed of the animation.
        std::vector<float> elapsed;                  /// < The elapsed time since the beginning of the animation.
        std::vector<float> timePerFrame;             /// < Shows how long a frame should be drawed.
        std::vector<float> spacing;                  /// < The spacing between the frames of the sprites, 0 if none.
        std::vector<int> current;                    /// < The index of the current frame.
        std::vector<object> running;                 /// < The id-s of the running animations.
    } animations;
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_ANIMATED_OBJECTS_HPP
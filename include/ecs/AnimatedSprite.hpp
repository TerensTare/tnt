#ifndef TNT_ECS_ANIMATED_SPRITE_HPP
#define TNT_ECS_ANIMATED_SPRITE_HPP

#include "ecs/Sprite.hpp"

namespace tnt
{
    namespace detail
    {
        class animation_data;
    } // namespace detail

    /// @brief The @c Component that brings animation to @c Object.
    class Animator final : public Component
    {
    public:
        /// @brief The wrap mode of the animation
        enum WrapMode
        {
            SINGLE_RUN = 0, ///< Run only once
            LOOP            ///< Repeat forever
        };

        /// @brief The direction at which animation frames are arranged on the spritesheet.
        enum Direction
        {
            HORIZONTAL = 0, ///< Frames are horizontally arranged
            VERTICAL        ///< Frames are vertically arranged
        };

        /// @brief Create an animation used when the @c Object is on idle state.
        /// The name of this animation is set to "idle".
        /// @param rect The location of the frame to be set as idle.
        explicit Animator(Rectangle const &rect) noexcept;

        /// @brief Add a new animation for the @c Object.
        /// @param name The name to be assigned to the new animation.
        /// @param x The x-position of the first frame of the animation on the spritesheet.
        /// @param y The y-position of the first frame of the animation on the spritesheet.
        /// @param frames The number of frames the animation has.
        /// @param speed The speed at which the animation is played.
        /// @param dir The direction of the frames on the spritesheet(horizontal or vertical).
        /// @param mode The wrap mode of the animation.
        void addAnimation(std::string_view name, float x, float y,
                          int frames, long long speed, Direction const &dir,
                          WrapMode const &mode);

        /// @brief Stop the current animation and run @c name animation.
        /// @param name The name of the animation to play.
        void playAnimation(std::string_view name) noexcept;

        /// @brief Return the name of the current animation.
        /// @return std::string
        std::string lastAnimation() const noexcept;

    private:
        std::string current;
        std::map<std::string, detail::animation_data *> animations;
    };

    namespace detail
    {
        class animation_data final
        {
        public:
            animation_data(Rectangle const &rect,
                           int framesCount, long long animSpeed,
                           Animator::Direction const &direction,
                           Animator::WrapMode const &wrap) noexcept;

            void Update(long long time_) noexcept;

            void finish() noexcept;
            bool running() const noexcept;

            void reset() noexcept;

            Rectangle Area() const noexcept;

        private:
            bool finished{false};
            Animator::WrapMode wrapMode;
            Animator::Direction dir;
            float startX, startY;
            Rectangle clip;
            long long elapsedTime{0};
            long long speed, timePerFrame;
        };
    } // namespace detail

    /// @brief A basic animated @c Object.
    class AnimatedSprite : public Sprite
    {
    public:
        /// @brief Create an @c Object with a default idle animation.
        /// @param win The window where the @c Sprite should be drawed.
        /// @param filename The name of the file to be used as a spritesheet.
        /// @param rect The area of the "idle"(default) frame on the spritesheet.
        AnimatedSprite(
            tnt::Window const *win, std::string_view filename,
            Rectangle const &rect) noexcept;

        Animator *getAnimator() const noexcept;

        /// @brief Update the current animation.
        /// @param time_ The time elapsed since the last @c Update call.
        void Animate(long long time_) noexcept;

    private:
        Animator *animator;
    };
} // namespace tnt

#endif //!TNT_ECS_ANIMATED_SPRITE_HPP
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ecs/AnimatedSprite.hpp"

tnt::detail::animation_data::animation_data(
    tnt::Rectangle const &rect, int framesCount,
    long long animSpeed, tnt::Animator::Direction const &direction,
    tnt::Animator::WrapMode const &wrap) noexcept
    : clip{rect}, startX{rect.x}, startY{rect.y},
      speed{animSpeed}, timePerFrame{animSpeed / framesCount},
      dir{direction}, wrapMode{wrap} {}

void tnt::detail::animation_data::Update(float time_) noexcept
{
    [[likely]] if (!finished)
    {
        elapsedTime = elapsedTime + time_;

        if (elapsedTime >= speed)
        {
            [[likely]] if (wrapMode == Animator::LOOP)
                elapsedTime = elapsedTime - speed;
            else [[unlikely]]
            {
                finished = true;
                elapsedTime = speed - timePerFrame;
            }
        }

        if (dir == Animator::HORIZONTAL)
            clip.x = startX + static_cast<float>(elapsedTime / timePerFrame) * clip.w;
        else
            clip.y = startY + static_cast<float>(elapsedTime / timePerFrame) * clip.h;
    }
}

void tnt::detail::animation_data::finish() noexcept { finished = true; }
bool tnt::detail::animation_data::running() const noexcept { return !finished; }

void tnt::detail::animation_data::reset() noexcept
{
    if (finished)
        finished = false;
    clip.x = startX;
    clip.y = startY;
    elapsedTime = 0;
}

tnt::Rectangle tnt::detail::animation_data::Area() const noexcept { return clip; }

tnt::Animator::Animator(Rectangle const &rect) noexcept
    : current{"idle"}
{
    animations.try_emplace("idle", std::move(detail::animation_data{rect, 1, 1, Animator::HORIZONTAL, Animator::SINGLE_RUN}));
}

void tnt::Animator::addAnimation(
    std::string_view name, float x, float y, int frames,
    long long speed, Animator::Direction const &dir,
    Animator::WrapMode const &mode)
{
    animations.try_emplace(name.data(), std::move(detail::animation_data{{x, y, w, h}, frames, speed, dir, mode}));
}

void tnt::Animator::playAnimation(std::string_view name) noexcept
{
    if (detail::animation_data * curr{animations[current]}; curr->running())
        curr->finish();

    current = name;
    if (detail::animation_data * curr{animations[current]}; curr->running())
        curr->reset();
}

std::string tnt::Animator::lastAnimation() const noexcept { return current; }

tnt::AnimatedSprite::AnimatedSprite(
    tnt::Window const *win, std::string_view filename,
    Rectangle const &rect) noexcept
    : Sprite{win, filename, rect},
      animator{add<Animator>(rect)} {}

tnt::Animator *tnt::AnimatedSprite::getAnimator() const noexcept { return animator; }

void tnt::AnimatedSprite::Animate(float time_) noexcept
{
}
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Animations.hpp"
#include "doo_ecs/Sprites.hpp"
#include "json/JsonRectangle.hpp"
#include "utils/TypeUtils.hpp"

namespace tnt::doo
{
    constexpr animation_comp::animation_comp(tnt::Rectangle const &rect,
                                             int const framesCount, float const animSpeed, float const space,
                                             direction const &dir_, wrap_mode const &wrap_) noexcept
        : clip{rect}, startX{rect.x}, startY{rect.y},
          speed{animSpeed}, timePerFrame{animSpeed / framesCount},
          spacing{space}, dir{dir_}, wrap{wrap_} {}

    void animations_sys::add_object(animation_comp const &anim_)
    {
        object const &index{wrap.size()};
        [[unlikely]] if (index == wrap.capacity())
        {
            wrap.reserve(10);
            dir.reserve(10);
            startX.reserve(10);
            startY.reserve(10);
            elapsed.reserve(10);
            speed.reserve(10);
            timePerFrame.reserve(10);
            spacing.reserve(10);
            current.reserve(10);
            running.reserve(10);
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
    }

    void animations_sys::add_invalid()
    {
        object const &index{wrap.size()};
        [[unlikely]] if (index == wrap.capacity())
        {
            wrap.reserve(10);
            dir.reserve(10);
            startX.reserve(10);
            startY.reserve(10);
            elapsed.reserve(10);
            speed.reserve(10);
            timePerFrame.reserve(10);
            spacing.reserve(10);
            current.reserve(10);
            running.reserve(10);
        }

        wrap.emplace_back(animation_comp::wrap_mode::loop);
        dir.emplace_back(animation_comp::direction::horizontal);
        startX.emplace_back(0.f);
        startY.emplace_back(0.f);
        elapsed.emplace_back(0.f);
        speed.emplace_back(0.f);
        timePerFrame.emplace_back(0.f);
        spacing.emplace_back(0.f);
        current.emplace_back(0);

        running.emplace_back(-1);
    }

    void animations_sys::Update(object const &id, float time_) noexcept
    {
        elapsed[id] += time_;
        if (elapsed[id] - (timePerFrame[id] * (current[id] + 1)) >= FLT_EPSILON)
        {
            ++current[id];
            if (int const &index_{current[id] % int(speed[id] / timePerFrame[id])};
                wrap[id] == animation_comp::loop)
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
                elapsed[id] = speed[id] - timePerFrame[id];
                sprites.clip[id].x = (int)startX[id];
                sprites.clip[id].y = (int)startY[id];
            }
        }
    }

    void animations_sys::from_json(nlohmann::json const &j)
    {
        if (json_has(j, "anim"))
        {
            Rectangle const &rect{j["sprite"]["crop"]};
            nlohmann::json const &chunk{j["anim"]};
            int const &frames{chunk["frames"]};
            float const &speed{chunk["speed"]};
            float const &space{
                if_else(json_has(chunk, "space"),
                        chunk["space"], 0.f)};
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
        else
            add_invalid();
    }
} // namespace tnt::doo
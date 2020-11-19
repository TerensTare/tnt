// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Animations.hpp"
#include "doo_ecs/Objects.hpp"
#include "doo_ecs/Sprites.hpp"

#include "json/JsonRectangle.hpp"
#include "utils/TypeUtils.hpp"

#include "utils/Assert.hpp"

namespace tnt::doo
{
    void animations_sys::add_object(object const &id, animation_comp const &anim_)
    {
        safe_ensure(objects.active.contains(id), "Adding inexistent object to animations_sys!!");
        safe_ensure(sprites.active.contains(id), "Adding object without sprite component to animations_sys!! Add the object to sprites_sys first!!");

        [[unlikely]] if (id > wrap.capacity())
        {
            wrap.reserve(id - wrap.capacity());
            dir.reserve(id - wrap.capacity());
            startX.reserve(id - wrap.capacity());
            startY.reserve(id - wrap.capacity());
            elapsed.reserve(id - wrap.capacity());
            speed.reserve(id - wrap.capacity());
            timePerFrame.reserve(id - wrap.capacity());
            spacing.reserve(id - wrap.capacity());
            current.reserve(id - wrap.capacity());
        }

        wrap.emplace(wrap.cbegin() + id, anim_.wrap);
        dir.emplace(dir.cbegin() + id, anim_.dir);
        startX.emplace(startX.cbegin() + id, anim_.startX);
        startY.emplace(startY.cbegin() + id, anim_.startY);
        elapsed.emplace(elapsed.cbegin() + id, 0.f);
        speed.emplace(speed.cbegin() + id, anim_.speed);
        timePerFrame.emplace(timePerFrame.cbegin() + id, anim_.timePerFrame);
        spacing.emplace(spacing.cbegin() + id, anim_.spacing);
        current.emplace(current.cbegin() + id, 0);

        active.insert(id, id);
    }

    void animations_sys::Update(object const &id, float time_) noexcept
    {
        if (active.contains(id))
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
                    active.erase(id);
                    elapsed[id] = speed[id] - timePerFrame[id];
                    sprites.clip[id].x = (int)startX[id];
                    sprites.clip[id].y = (int)startY[id];
                }
            }
        }
    }

    void animations_sys::from_json(object const &id, nlohmann::json const &j)
    {
        if (j.contains("anim"))
        {
            Rectangle const &rect{j["sprite"]["crop"]};
            nlohmann::json const &chunk{j["anim"]};
            int const &frames{chunk["frames"]};
            float const &speed{chunk["speed"]};
            float const &space{if_else(chunk.contains("space"),
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

            add_object(id, animation_comp{rect, frames, speed, space, dir_, wrap_});
        }
    }

    void animations_sys::remove(object const &id) noexcept
    {
        active.erase(id);
        wrap.erase(wrap.cbegin() + id);
        dir.erase(dir.cbegin() + id);
        startX.erase(startX.cbegin() + id);
        startY.erase(startY.cbegin() + id);
        elapsed.erase(elapsed.cbegin() + id);
        speed.erase(speed.cbegin() + id);
        timePerFrame.erase(timePerFrame.cbegin() + id);
        spacing.erase(spacing.cbegin() + id);
        current.erase(current.cbegin() + id);
    }

    void animations_sys::clear() noexcept
    {
        active.clear();
        wrap.clear();
        dir.clear();
        startX.clear();
        startY.clear();
        elapsed.clear();
        speed.clear();
        timePerFrame.clear();
        spacing.clear();
        current.clear();
    }
} // namespace tnt::doo
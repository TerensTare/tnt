// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Cameras.hpp"
#include "doo_ecs/Objects.hpp"

#include "imgui/ImGui.hpp"
#include "json/JsonVector.hpp"
#include "pcg/Random.hpp"

namespace tnt::doo
{
    void cameras_sys::add_object(camera_comp const &data)
    {
        [[unlikely]] if (angle.size() == angle.capacity())
        {
            angle.reserve(3);
            width.reserve(3);
            height.reserve(3);
            speed.reserve(3);
            shaking.reserve(3);
            shakeLoss.reserve(3);
            shake_time.reserve(3);
            active.reserve(3);
            target.reserve(3);
            pos.reserve(3);
            scale.reserve(3);
            offset.reserve(3);
        }

        active.emplace_back(angle.size());
        angle.emplace_back(data.angle);
        width.emplace_back(data.w);
        height.emplace_back(data.h);
        speed.emplace_back(data.speed);
        shaking.emplace_back(data.shaking);
        shakeLoss.emplace_back(data.shakeLoss);
        shake_time.emplace_back(-1.f);
        target.emplace_back(null);
        pos.emplace_back(data.pos);
        scale.emplace_back(data.scale);
        offset.emplace_back(0.f, 0.f);
    }

    void cameras_sys::from_json(nlohmann::json const &j)
    {
        float angle{0.f};
        float speed_{5.f};
        float shaking_{.7f};
        float shake_loss_{1.f};
        Vector scale_{1.f, 1.f};

        if (json_has(j, "angle"))
            angle = j["angle"];
        if (json_has(j, "speed"))
            speed_ = j["speed"];
        if (json_has(j, "scale"))
            scale_ = j["scale"];
        if (json_has(j, "shake"))
            shaking_ = j["shake"];
        if (json_has(j, "shake_loss"))
            shake_loss_ = j["shake_loss"];

        add_object({angle, j["pos"].get<Vector>(),
                    j["w"].get<float>(), j["h"].get<float>(),
                    speed_, shaking_, shake_loss_, scale_});
    }

    void cameras_sys::draw_imgui(camera const &id, Window const &win) noexcept
    {
        // TODO: from-player offset
        if (tnt::ImGui::BeginSection(win, "Camera"))
        {
            tnt::ImGui::hslider_float(win, "Rotation", -360.f, 360.f, &angle[id]);
            tnt::ImGui::hslider_vec(win, "Position", 0.f, 2000.f, 0.f, 2000.f, &pos[id]);
            tnt::ImGui::hslider_float(win, "Width", 0.f, 2000.f, &width[id]);
            tnt::ImGui::hslider_float(win, "Height", 0.f, 2000.f, &height[id]);
            tnt::ImGui::hslider_float(win, "Follow Speed", 0.f, 20.f, &speed[id]);
            tnt::ImGui::hslider_float(win, "Shaking factor", 0.f, 1.f, &shaking[id]);
            tnt::ImGui::hslider_float(win, "Shaking loss", 0.f, 1.f, &shakeLoss[id]);
        }
    }

    void cameras_sys::follow(camera const &cam, object const &id, float time_, Vector const &off) noexcept
    {
        Vector const &target_{objects.gPos(id) - off};
        Vector const &half_wh{width[cam] * .5f, height[cam] * .5f};
        pos[cam] = lerp<Vector>(pos[cam], target_ - half_wh, time_ * speed[cam] * .001f);
    }

    void cameras_sys::shake(camera const &cam) noexcept
    {
        pos[cam] += randomUnitVector() * shaking[cam];
    }
} // namespace tnt::doo
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <execution>
#include <nlohmann/json.hpp>

#include "doo_ecs/Cameras.hpp"
#include "doo_ecs/Objects.hpp"

#include "imgui/ImGui.hpp"
#include "json/JsonVector.hpp"
#include "pcg/Random.hpp"
#include "utils/Benchmark.hpp"

namespace tnt::doo
{
    void cameras_sys::add_object(camera_comp const &data)
    {
        PROFILE_FUNCTION();

        [[unlikely]] if (angle.size() == angle.capacity())
        {
            angle.reserve(3);
            width.reserve(3);
            height.reserve(3);
            speed.reserve(3);
            shaking.reserve(3);
            shakeLoss.reserve(3);
            shake_time.reserve(3);
            target.reserve(3);
            pos.reserve(3);
            scale.reserve(3);
            offset.reserve(3);
        }

        active.push_back((camera)angle.size());
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
        PROFILE_FUNCTION();

        float angle_{0.f};
        float speed_{5.f};
        float shaking_{.7f};
        float shake_loss_{1.f};
        Vector scale_{1.f, 1.f};

        if (j.contains("angle"))
            angle_ = j["angle"];
        if (j.contains("speed"))
            speed_ = j["speed"];
        if (j.contains("scale"))
            scale_ = j["scale"];
        if (j.contains("shake"))
            shaking_ = j["shake"];
        if (j.contains("shake_loss"))
            shake_loss_ = j["shake_loss"];

        add_object({angle_, j["pos"].get<Vector>(),
                    j["w"].get<float>(), j["h"].get<float>(),
                    speed_, shaking_, shake_loss_, scale_});
    }

    void cameras_sys::to_json(camera const &id, nlohmann::json &j)
    {
        if (active.contains(id))
        {
            PROFILE_FUNCTION();

            nlohmann::json camera{j["camera"]};
            camera["angle"] = angle[id];
            camera["speed"] = speed[id];
            camera["scale"] = scale[id];
            camera["shake"] = shaking[id];
            camera["shake_loss"] = shakeLoss[id];
        }
    }

    void cameras_sys::draw_imgui(camera const &id, Window const &win) noexcept
    {
        PROFILE_FUNCTION();

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
        PROFILE_FUNCTION();

        Vector const &target_{objects.gPos(id) - off};
        Vector const &half_wh{width[cam] * .5f, height[cam] * .5f};
        pos[cam] = lerp<Vector>(pos[cam], target_ - half_wh, time_ * speed[cam] * .001f);
    }

    void cameras_sys::shake(camera const &cam) noexcept
    {
        PROFILE_FUNCTION();

        pos[cam] += randomUnitVector() * shaking[cam];
    }

    void cameras_sys::zoom_to_fit(camera const &cam, std::span<object> objs)
    {
        PROFILE_FUNCTION();

        // TODO: not *perfect*. Ex. A corner may be left outside.
        Vector min_{objects.gPos(objs.front())};
        Vector max_{objects.gPos(objs.front())}, curr; // first value
        for (auto const &obj : objs)
        {
            curr = objects.gPos(obj);
            if (curr < min_)
                min_ = curr;
            if (curr > max_)
                max_ = curr;
        }
        pos[cam] = min_;

        // TODO: change scale here
        width[cam] = max_.x - min_.x;  // (maybe) * cosf(gAngle(obj)-angle[cam])
        height[cam] = max_.y - min_.y; // (maybe) * sinf(gAngle(obj)-angle[cam])
    }

    void cameras_sys::remove(camera const &id) noexcept
    {
        PROFILE_FUNCTION();

        active.erase(id);
        angle.erase(angle.cbegin() + id);
        width.erase(width.cbegin() + id);
        height.erase(height.cbegin() + id);
        speed.erase(speed.cbegin() + id);
        shaking.erase(shaking.cbegin() + id);
        shakeLoss.erase(shakeLoss.cbegin() + id);
        shake_time.erase(shake_time.cbegin() + id);
        target.erase(target.cbegin() + id);
        pos.erase(pos.cbegin() + id);
        scale.erase(scale.cbegin() + id);
        offset.erase(offset.cbegin() + id);
    }

    void cameras_sys::clear() noexcept
    {
        PROFILE_FUNCTION();

        active.clear();
        angle.clear();
        width.clear();
        height.clear();
        speed.clear();
        shaking.clear();
        shakeLoss.clear();
        shake_time.clear();
        target.clear();
        pos.clear();
        scale.clear();
        offset.clear();
    }
} // namespace tnt::doo
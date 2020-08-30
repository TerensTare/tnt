// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "doo_ecs/Base.hpp"
#include "json/JsonVector.hpp"

namespace tnt::doo
{
    constexpr object_data::object_data(float angle_, Vector const &pos_, Vector const &scale_) noexcept
        : angle{angle_}, pos{pos_}, scale{scale_} {}

    object objects_sys::add_object(object_data const &data_) noexcept
    {
        object const &index{angle.size()};

        [[unlikely]] if (index == angle.capacity()) // if the vector got to his capacity, then we reserve more space
        {
            active.reserve(10);
            angle.reserve(10);
            scale.reserve(10);
            pos.reserve(10);
        }

        active.emplace_back(index);
        angle.emplace_back(data_.angle);
        scale.emplace_back(data_.scale);
        pos.emplace_back(data_.pos);

        return index;
    }

    [[nodiscard]] object_data objects_sys::get_data(object const &id) const noexcept
    {
        return object_data{angle[id], pos[id], scale[id]};
    }

    void objects_sys::from_json(nlohmann::json const &j)
    {
        add_object({j["angle"].get<float>(),
                    j["pos"].get<Vector>(),
                    j["scale"].get<Vector>()});
    }
} // namespace tnt::doo
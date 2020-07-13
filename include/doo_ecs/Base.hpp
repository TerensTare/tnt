#ifndef TNT_DOO_ECS_BASE_HPP
#define TNT_DOO_ECS_BASE_HPP

namespace tnt::doo
{
    using object = std::size_t;

    template <typename T>
    concept system = requires(T const &t)
    {
        typename T::add_object;
        typename T::Update;
    };

    class object_data
    {
    public:
        float angle;
        Vector pos;
        Vector scale;
    };
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_BASE_HPP
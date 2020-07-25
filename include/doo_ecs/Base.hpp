#ifndef TNT_DOO_ECS_BASE_HPP
#define TNT_DOO_ECS_BASE_HPP

namespace tnt::doo
{
    using object = std::size_t; /// < A data type that holds an unique id of an object

    template <typename T>
    concept system = requires(T const &t)
    {
        typename T::add_object;
        typename T::Update;
    };

    /// @brief A struct that holds the basic data of an object.
    struct object_data
    {
        /// @brief Create a new object data handle from the given params.
        /// @param angle_ The initial angle of the object.
        /// @param scale_ The initial scale of the object.
        /// @param pos_ The initial position of the object.
        inline constexpr object_data(float angle_, Vector const &pos_, Vector const &scale_) noexcept
            : angle{angle_}, pos{pos_}, scale{scale_} {}

        /// @brief Create a new object data handle with the default values.
        inline constexpr object_data() noexcept
            : angle{0.f}, pos{0.f, 0.f}, scale{0.f, 0.f} {}

        float angle;  /// < The angle of the object.
        Vector scale; /// < The scale of the object.
        Vector pos;   /// < The position of the object.
    };
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_BASE_HPP
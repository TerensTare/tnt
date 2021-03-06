#ifndef TNT_UTILS_SORT_HPP
#define TNT_UTILS_SORT_HPP

#include <algorithm>
#include <concepts>
#include <ranges>
#include <span>

#include "types/TypeUtils.hpp"

// TODO: unfinished
// TODO: write this header using std::sortable types.

namespace tnt
{
    template <std::ranges::input_range R>
    class bubble_sort_view
        : public std::ranges::view_interface<bubble_sort_view<R>>
    {
        R base_{};

    public:
        bubble_sort_view() noexcept = default;

        // clang-format off
        explicit constexpr bubble_sort_view(R &&r)
            requires std::move_constructible<R> &&
            std::three_way_comparable<std::ranges::range_value_t<R>>
            // clang-format on
            : base_{std::move(r)}
        {
            for (std::size_t i{0}; i < base_.size(); ++i)
                for (std::size_t j{0}; j < base_.size() - 1; ++j)
                    if (base_[j] > base_[j + 1])
                    {
                        if constexpr (std::swappable<std::ranges::range_value_t<R>>)
                            std::swap(base_[j], base_[j + 1]);
                        else
                        {
                            std::ranges::range_value_t<R> const &temp{base_[j]};
                            base_[j] = base_[j + 1];
                            base_[j + 1] = temp;
                        }
                    }
        }

        constexpr iterator begin() const noexcept { return std::ranges::begin(base_); }
        constexpr auto end() const noexcept { return std::ranges::end(base_); }

        constexpr iterator cbegin() const noexcept { return std::ranges::cbegin(base_); }
        constexpr auto cend() const noexcept { return std::ranges::cend(base_); }
    };

    template <std::ranges::input_range R>
    bubble_sort_view(R &&) noexcept
        ->bubble_sort_view<std::ranges::views::all_t<R>>;

    struct bubble_sort_fn final
    {
        template <std::ranges::input_range R>
        constexpr bubble_sort_view<std::ranges::views::all_t<R>>
        operator()(R &&rng) const noexcept
        {
            return {std::ranges::views::all(static_cast<R &&>(rng))};
        }
    };

    template <std::ranges::input_range R>
    constexpr bubble_sort_view<std::ranges::views::all_t<R>>
    operator|(R &&rng, bubble_sort_fn const &) noexcept
    {
        return {std::ranges::views::all(static_cast<R &&>(rng))};
    }

    namespace views
    {
        inline constexpr bubble_sort_fn bubble_sort{};
    }
} // namespace tnt

#endif //! TNT_UTILS_SORT_HPP
#ifndef TNT_ASSERT_UTILITIES_HPP
#define TNT_ASSERT_UTILITIES_HPP

#include "utils/Logger.hpp"
#include "types/TypeUtils.hpp"

// STATIC_CHECK is used for compile time only.
#define STATIC_CHECK(expn) typedef char __C_ASSERT__##expn[(expn) ? 1 : -1]

// TODO: boolean concept for ensure

// TODO(maybe):
// remove the why parameter from failed_ensure ?? (you already know why)

namespace tnt
{
    /// @brief Exception thrown if the condition of ensure_t::operator() is false.
    struct failed_ensure final : std::exception
    {
        inline explicit failed_ensure(char const *why) noexcept : reason{why} {}

        [[nodiscard]] inline char const *what() const noexcept override { return reason; }

    private:
        char const *reason;
    };

    /// @brief Utility struct used for checking preconditions and/or postconditions.
    inline overload ensure{
        [](bool cond) {
            if (!cond)
                throw failed_ensure{""};
        },
        [](bool cond, char const *message) {
            if (!cond)
                throw failed_ensure{message};
        }};

    /// @brief A version of ensure_t that calls std::terminate on false predication.
    inline overload safe_ensure{
        [](bool cond) noexcept {
            if (!cond)
                std::terminate();
        },
        [](bool cond, char const *message) noexcept {
            if (!cond)
                logger::error(message);
        }};

    /// @brief A version of ensure_t that just prints a warning on false predicate.
    inline auto check = [](bool cond, char const *message) noexcept {
        if (!cond)
            logger::warning("Predicate failed!! {}", message);
    };
} // namespace tnt

#endif //!TNT_ASSERT_UTILITIES_HPP
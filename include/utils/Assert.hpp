#ifndef TNT_ASSERT_UTILITIES_HPP
#define TNT_ASSERT_UTILITIES_HPP

#include "utils/Logger.hpp"

// STATIC_CHECK is used for compile time only.
#define STATIC_CHECK(expn) typedef char __C_ASSERT__##expn[(expn) ? 1 : -1]

// TODO: constinit ensure, constexpr ctors, etc
// TODO: boolean concept for ensure_t

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
    inline struct ensure_t final
    {
        explicit ensure_t() noexcept = default;

        /// @brief Throw a failed_ensure if cond evaluates to false.
        /// @param cond The condition to evaluate.
        inline void operator()(bool cond)
        {
            if (!cond)
                throw failed_ensure{""};
        }

        /// @brief Throw a failed_ensure if cond evaluates to false.
        /// @param cond The condition to evaluate.
        /// @param message The message that the thrown exception will hold.
        inline void operator()(bool cond, char const *message)
        {
            if (!cond)
                throw failed_ensure{message};
        }
    } ensure;

    /// @brief A version of ensure_t that calls std::terminate on false predication.
    inline struct safe_ensure_t final
    {
        explicit safe_ensure_t() noexcept = default;

        /// @brief Terminate the process if cond evaluates to false.
        /// @param cond The condition to evaluate.
        inline void operator()(bool cond) noexcept
        {
            if (!cond)
                std::terminate();
        }

        /// @brief Terminate the process if cond evaluates to false.
        /// @param cond The condition to evaluate.
        /// @param message The message to print on the console.
        inline void operator()(bool cond, char const *message) noexcept
        {
            if (!cond)
                logger::error(message);
        }
    } safe_ensure;

    /// @brief A version of ensure_t that just prints a warning on false predicate.
    inline struct check_t final
    {
        explicit check_t() noexcept = default;

        /// @brief Print message to the console as an info, if cond evaluates to false.
        /// @param cond The condition to evaluate.
        /// @param message The message to print on the console.
        inline void operator()(bool cond, char const *message) noexcept
        {
            if (!cond)
                logger::warning("Predication failed!! {}", message);
        }
    } check;
} // namespace tnt

#endif //!TNT_ASSERT_UTILITIES_HPP
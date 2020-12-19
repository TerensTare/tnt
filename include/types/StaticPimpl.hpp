#ifndef TNT_PIMPL_ON_STACK_IDIOM_HPP
#define TNT_PIMPL_ON_STACK_IDIOM_HPP

#include <type_traits>
#include "core/Config.hpp"

// TODO: pimpl.emplace

namespace tnt
{
    // thx Antony Polukhin and kuznetsss
    // https://youtu.be/SYqpeMW8Pek?t=30978
    // https://github.com/kuznetsss/pimpl_on_stack/blob/master/include/pimpl_on_stack/pimpl_on_stack.h
    template <typename T, std::size_t Size, std::size_t Align>
    class TNT_API pimpl final
    {
    public:
        template <typename... Args>
        inline explicit pimpl(Args &&... args)
            : ptr{reinterpret_cast<T *>(&storage)}
        {
            new (ptr) T{std::forward<Args>(args)...};
        }

        inline pimpl(pimpl &&other) noexcept
            : ptr{reinterpret_cast<T *>(&storage)}
        {
            *this = std::move(other);
        }

        inline pimpl &operator=(pimpl<T, Size, Align> &&other) noexcept
        {
            ptr = reinterpret_cast<T *>(&storage);
            ptr = std::exchange(other.ptr, nullptr);
            return *this;
        }

        pimpl(pimpl const &) = delete;
        pimpl &operator=(pimpl const &) = delete;

        inline ~pimpl()
        {
            check<Size, Align>();
            reset();
        }

        inline T &operator*() noexcept { return *ptr; }
        inline const T &operator*() const noexcept { return *ptr; }

        inline T *operator&() noexcept { return ptr; }
        inline const T *operator&() const noexcept { return ptr; }

        inline T *operator->() noexcept { return ptr; }
        inline const T *operator->() const noexcept { return ptr; }

        inline bool operator==(std::nullptr_t) const noexcept { return ptr == nullptr; }

        [[nodiscard]] inline bool empty() const noexcept { return ptr == nullptr; }

        inline void reset() noexcept(std::is_nothrow_destructible_v<T>)
        {
            if (ptr != nullptr)
            {
                ptr->~T();
                ptr = nullptr;
            }
        }

    private:
        template <std::size_t S1, std::size_t A1,
                  std::size_t S2 = sizeof(T),
                  std::size_t A2 = alignof(T)>
        static constexpr void check() noexcept
        {
            static_assert(S1 == S2, "Incorrect size applied to tnt::pimpl!!");
            static_assert(A1 == A2, "Incorrect alignment applied to tnt::pimpl!!");
        }

        T *ptr;
        std::aligned_storage_t<Size, Align> storage;
    };
} // namespace tnt

#endif //!TNT_PIMPL_ON_STACK_IDIOM_HPP
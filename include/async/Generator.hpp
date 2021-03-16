#ifndef TNT_ASYNCHRONOUS_COROUTINE_GENERATOR_HPP
#define TNT_ASYNCHRONOUS_COROUTINE_GENERATOR_HPP

#include <coroutine>
#include <memory_resource>
#include <ranges>

// TODO:
// better constraints for the allocator.
// integrate this to the job system
// invoke_coro, for types with custom allocator support

// TODO(maybe):
// recursive_generator, which has a yield_value(recursive_generator)/yield_value(generator) overload ??
// iterator comparison ??

namespace tnt
{
    namespace detail
    {
        template <typename Alloc, typename T>
        concept allocator_for = requires
        {
            requires std::same_as<typename std::allocator_traits<Alloc>::value_type, T>;
        };
    }

    // thx Kirit Sælensminde and Park DongHa
    // https://kirit.com/How%20C%2B%2B%20coroutines%20work/A%20more%20realistic%20coroutine
    // https://luncliff.github.io/posts/Exploring-MSVC-Coroutine.html
    template <std::copyable T, detail::allocator_for<std::byte> Alloc = std::pmr::polymorphic_allocator<std::byte>>
    class [[nodiscard]] generator final
    {
    public:
        struct promise_type;
        struct iterator;

        using allocator_type = Alloc;

        constexpr generator() noexcept
            : coro{} {}

        generator(generator const &) = delete;
        generator &operator=(generator const &) = delete;

        constexpr generator(generator &&rhs) noexcept
            : coro{std::exchange(rhs.coro, nullptr)} {}

        constexpr generator &operator=(generator &&rhs) noexcept
        {
            if (this != std::addressof(rhs))
                coro = std::exchange(rhs.coro, nullptr);
            return *this;
        }

        inline ~generator() noexcept
        {
            if (coro)
                coro.destroy();
        }

        inline T current_value() const noexcept { return *coro.promise().value; }

        inline bool move_next() noexcept
        {
            coro.resume();
            return coro.done();
        }

        inline iterator begin() noexcept
        {
            if (coro)
            {
                coro.resume();

                if (coro.done())
                    return iterator{};
            }

            return iterator{coro};
        }

        constexpr std::default_sentinel_t end() const noexcept { return {}; }

        inline operator bool() const noexcept
            requires std::convertible_to<T, bool>
        {
            return *coro.promise().value;
        }

    private:
        explicit constexpr generator(std::coroutine_handle<promise_type> h) noexcept
            : coro{h} {}

        std::coroutine_handle<promise_type> coro;
    };

    template <std::copyable T, detail::allocator_for<std::byte> Alloc>
    struct generator<T, Alloc>::promise_type final
    {
        constexpr std::suspend_always initial_suspend() const noexcept { return {}; }
        constexpr std::suspend_always final_suspend() const noexcept { return {}; }

        inline generator<T, Alloc> get_return_object() noexcept
        {
            return generator<T, Alloc>{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        [[noreturn]] inline void unhandled_exception() const noexcept { std::terminate(); }
        constexpr std::suspend_never return_void() const noexcept { return {}; }

        inline std::suspend_always yield_value(T const &val) noexcept
        {
            value = std::addressof(val);
            return {};
        }

        void await_transform() = delete; // no co_await

        // template <typename... Args>
        // inline static void *operator new(std::size_t size, std::allocator_arg_t, Alloc &&alloc, Args &...)
        // {
        //     void *ptr = alloc.allocate(size);
        //     promise_type &promise = *(promise_type *)ptr;
        //     promise.alloc = std::move(alloc);
        //
        //     return ptr;
        // }

        // inline static void* operator new(std::size_t size)
        // {
        //     Alloc alloc{};
        //     void *ptr = alloc.allocate(size);
        //     promise_type& promise = *(promise_type *)ptr;
        //     promise.alloc = std::move(alloc);
        //
        //     return ptr;
        // }

        // inline static void operator delete(void *ptr, std::size_t size)
        // {
        //     promise_type *promise = (promise_type *)ptr;
        //     Alloc alloc = std::move(promise->alloc);
        //     alloc.deallocate(promise, size);
        // }

    private:
        T const *value;
        Alloc alloc;

        friend generator<T, Alloc>;
    };

    template <std::copyable T, detail::allocator_for<std::byte> Alloc>
    struct generator<T, Alloc>::iterator final
    {
        using iterator_category = std::input_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        constexpr iterator() noexcept
            : coro{} {}

        inline iterator(std::coroutine_handle<typename generator<T, Alloc>::promise_type> handle) noexcept
            : coro{handle} {}

        inline iterator &operator++() noexcept
        {
            coro.resume();
            if (coro.done())
                coro = nullptr;
            return *this;
        }

        inline void operator++(int) noexcept
        {
            coro.resume();
            if (coro.done())
                coro = nullptr;
        }

        inline T const &operator*() const noexcept { return *coro.promise().value; }
        inline T const *operator->() const noexcept { return coro.promise().value; }

        friend bool operator==(iterator const &it, std::default_sentinel_t) noexcept
        {
            return (not it.coro or it.coro.done());
        }

        friend bool operator==(std::default_sentinel_t, iterator const &it) noexcept
        {
            return (not it.coro or it.coro.done());
        }

    private:
        std::coroutine_handle<typename generator<T, Alloc>::promise_type> coro;
    };

    namespace pmr
    {
        template <std::copyable T>
        using generator = tnt::generator<T, std::pmr::polymorphic_allocator<std::byte>>;
    }
} // namespace tnt

template <std::copyable T, typename Alloc, typename... Args>
struct std::coroutine_traits<tnt::generator<T, Alloc>, std::allocator_arg_t, Alloc, Args...>
{
    using promise_type = typename tnt::generator<T, Alloc>::promise_type;
};

template <std::copyable T, tnt::detail::allocator_for<std::byte> Alloc>
inline constexpr bool std::ranges::enable_view<tnt::generator<T, Alloc>> = true;

#endif //!TNT_ASYNCHRONOUS_COROUTINE_GENERATOR_HPP
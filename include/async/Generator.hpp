#ifndef TNT_ASYNCHRONOUS_COROUTINE_GENERATOR_HPP
#define TNT_ASYNCHRONOUS_COROUTINE_GENERATOR_HPP

#include <coroutine>
#include <utility>

namespace tnt
{
    // thx Kirit Sælensminde and Park DongHa
    // https://kirit.com/How%20C%2B%2B%20coroutines%20work/A%20more%20realistic%20coroutine
    // https://luncliff.github.io/posts/Exploring-MSVC-Coroutine.html
    template <typename T>
    class generator final
    {
    public:
        struct promise_type;
        struct iterator;

        generator(generator const &) = delete;

        inline generator(generator &&g)
            : coro{std::exchange(g.coro, {})} {}

        inline ~generator() noexcept
        {
            if (coro)
                coro.destroy();
        }

        inline T current_value() const noexcept { return coro.promise().value; }

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
                    return {nullptr};
            }

            return {coro};
        }

        inline iterator end() const noexcept { return {nullptr}; }

    private:
        inline explicit generator(std::coroutine_handle<promise_type> h)
            : coro{h} {}

        std::coroutine_handle<promise_type> coro;
    };

    template <typename T>
    struct generator<T>::promise_type final
    {
        inline std::suspend_always initial_suspend() const noexcept { return {}; }
        inline std::suspend_always final_suspend() const noexcept { return {}; }

        inline generator<T> get_return_object() noexcept
        {
            return generator<T>{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        [[noreturn]] inline void unhandled_exception() const noexcept { std::terminate(); }

        inline std::suspend_never return_void() const noexcept { return {}; }

        inline std::suspend_always yield_value(T const &val) noexcept
        {
            value = std::addressof(val);
            return {};
        }

    private:
        T const *value;

        friend generator<T>;
    };

    template <typename T>
    struct generator<T>::iterator final
    {
        using iterator_category = std::input_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        inline iterator(std::nullptr_t) noexcept : coro{nullptr} {}
        inline iterator(std::coroutine_handle<typename generator<T, Alloc>::promise_type> handle) noexcept : coro{handle} {}

        inline iterator &operator++() noexcept
        {
            coro.resume();
            if (coro.done())
                coro = nullptr;
            return *this;
        }

        iterator operator++(int) = delete;

        inline T const &operator*() const noexcept { return *coro.promise().value; }
        inline T const *operator->() const noexcept { return coro.promise().value; }

        inline bool operator==(iterator const &rhs) const { return coro == rhs.coro; }

        std::coroutine_handle<typename generator<T>::promise_type> coro;
    };
} // namespace tnt

#endif //!TNT_ASYNCHRONOUS_COROUTINE_GENERATOR_HPP
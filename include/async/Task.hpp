#ifndef TNT_ASYNCHRONOUS_COROUTINE_TASK_HPP
#define TNT_ASYNCHRONOUS_COROUTINE_TASK_HPP

#include <coroutine>
#include <utility>

// TODO:
// deduction guides for task<T>.
// make await_resume return sth.
// custom allocators.
// test.

// TODO(maybe): specialisation for task<void> ??

namespace tnt
{
    // thx Lewiss Baker and Kirit Sælensminde
    // https://lewissbaker.github.io/2020/05/11/understanding_symmetric_transfer
    // https://kirit.com/How%20C%2B%2B%20coroutines%20work/A%20more%20realistic%20coroutine
    template <typename T>
    class task final
    {
    public:
        struct promise_type;
        struct awaiter;

        task(task const &) = delete;
        task &operator=(task const &) = delete;

        constexpr task(task &&t) noexcept
            : coro{std::exchange(t.coro, nullptr)} {}

        inline task &operator=(task &&t)
        {
            coro = std::exchange(t.coro, nullptr);
            return *this;
        }

        inline ~task() noexcept
        {
            if (coro)
                coro.destroy();
        }

        inline awaiter operator co_await() &&noexcept { return awaiter{coro}; }

        inline T get() const noexcept
        {
            if (not coro.done())
                coro.resume();
            return coro.promise().value;
        }

    private:
        explicit constexpr task(std::coroutine_handle<promise_type> h) noexcept
            : coro{h} {}

        std::coroutine_handle<promise_type> coro;
    };

    template <typename T>
    struct task<T>::promise_type final
    {
        struct final_awaiter final
        {
            constexpr bool await_ready() const noexcept { return false; }
            constexpr void await_resume() const noexcept {}

            inline std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> handle) const noexcept
            {
                return handle.promise().ctx;
            }
        };

        inline task<T> get_return_object() noexcept
        {
            return task<T>{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        constexpr std::suspend_always initial_suspend() const noexcept { return {}; }
        constexpr final_awaiter final_suspend() const noexcept { return {}; }

        inline std::suspend_never return_value(T val) noexcept
        {
            value = val;
            return {};
        }

        [[noreturn]] inline void unhandled_exception() const noexcept { std::terminate(); }

    private:
        std::coroutine_handle<> ctx;
        T value;

        friend task<T>;
    };

    template <typename T>
    struct task<T>::awaiter final
    {
        constexpr bool await_ready() const noexcept { return false; }

        inline std::coroutine_handle<> await_suspend(
            std::coroutine_handle<> ctx) noexcept
        {
            coro.promise().ctx = ctx;
            return coro;
        }

        constexpr void await_resume() const noexcept {}

    private:
        friend task<T>;

        explicit constexpr awaiter(std::coroutine_handle<typename task<T>::promise_type> handle) noexcept
            : coro{handle} {}

        std::coroutine_handle<typename task<T>::promise_type> coro;
    };
} // namespace tnt

#endif //!TNT_ASYNCHRONOUS_COROUTINE_TASK_HPP
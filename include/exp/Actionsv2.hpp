#ifndef TNT_ACTIONS_V2_IMPL_HPP
#define TNT_ACTIONS_V2_IMPL_HPP

#if __has_include(<experimental/generator>)
#include <experimental/generator>
#endif

#include <concepts>
#include <memory_resource>
#include <vector>

// TODO:
// remove #ifdef/#endif when library support for coroutines is available everywhere.
// separate implementation to a .cpp file.
// resumable loops DO NOT NEED to return bool (they run forever)

namespace tnt
{
#ifdef __cpp_lib_coroutines
    // based on Marcin Grzebieluch's talk
    // https://www.youtube.com/watch?v=vDA925C55F0
    struct action_result
    {
        struct promise_type
        {
            bool current_value;

            inline auto initial_suspend() noexcept { return std::experimental::suspend_always{}; }
            inline auto final_suspend() noexcept { return std::experimental::suspend_always{}; }
            [[noreturn]] inline void unhandled_exception() noexcept { return std::terminate(); }

            inline auto get_return_object()
            {
                return std::experimental::coroutine_handle<promise_type>::from_promise(*this);
            }

            inline auto yield_value(bool value_)
            {
                current_value = value_;
                return std::experimental::suspend_always{};
            }

            inline void return_void() const noexcept {}
        };

        std::experimental::coroutine_handle<promise_type> handle;

        inline action_result(std::experimental::coroutine_handle<promise_type> handle_)
            : handle{handle_} {}

        inline ~action_result() noexcept(noexcept(handle.destroy()))
        {
            if (handle)
                handle.destroy();
        }

        inline bool get_next() const
        {
            handle.resume();
            return handle.promise().current_value;
        }
    };
#endif

    template <typename T>
    concept timed_action = std::invocable<T, float, float>;

    template <typename T>
    concept loop_action = std::invocable<T, float>;

#ifdef __cpp_lib_coroutines
    template <typename T>
    concept noblock_timed_action = timed_action<T> &&
        std::same_as<std::invoke_result_t<T, float, float>,
                     action_result>;

    template <typename T>
    concept noblock_loop_action = loop_action<T> &&
        std::same_as<std::invoke_result_t<T, float>,
                     action_result>;

    template <typename T>
    concept noblock_action = noblock_timed_action<T> || noblock_loop_action<T>;
#endif

    template <typename T>
    concept block_timed_action = timed_action<T> &&
        std::same_as<std::invoke_result_t<T, float, float>, void>;

    template <typename T>
    concept block_loop_action = loop_action<T> &&
        std::same_as<std::invoke_result_t<T, float>, void>;

    template <typename T>
    concept blocking_action = block_timed_action<T> || block_loop_action<T>;

#ifdef __cpp_lib_coroutines
    template <typename T>
    concept action = noblock_action<T> || blocking_action<T>;
#else
    template <typename T>
    concept action = blocking_action<T>;
#endif

    using timed_action_fn = decltype([](float, float) {});
    using loop_action_fn = decltype([](float) {});

#ifdef __cpp_lib_coroutines
    using resumable_timed_action_fn = decltype([](float, float) -> action_result {});
    using resumable_loop_action_fn = decltype([](float) -> action_result {});
#endif

    class action_list final
    {
    public:
        action_list() = default;

        template <loop_action A>
        inline void add(A const &action_)
        {
#ifdef __cpp_lib_coroutines
            if constexpr (noblock_loop_action<A>)
            {
                loop_res.emplace_back(&action_);
                next.emplace_back(res_loop);
            }
            else
            {
                loop.emplace_back(&action_);
                next.emplace_back(block_loop);
            }
#else
            loop.emplace_back(&action_);
            next.emplace_back(block_loop);
#endif
        }

        template <timed_action A>
        inline void add(A const &action_, float total)
        {
#ifdef __cpp_lib_coroutines
            if constexpr (noblock_timed_action<A>)
            {
                timed_res.emplace_back(&action_);
                next.emplace_back(res_timed);
                total_res_timed.emplace_back(total);
            }
            else
            {
                timed.emplace_back(&action_);
                next.emplace_back(block_timed);
                total_timed.emplace_back(total);
            }
#else
            timed.emplace_back(&action_);
            next.emplace_back(block_timed);
            total_timed.emplace_back(total);
#endif
        }

        inline bool empty() const noexcept { return next.empty(); }

        inline void Update(float elapsed)
        {
            for (auto const &it : next)
            {
                if (it == block_timed)
                {
                    timed[0](elapsed, total_timed[0]);

                    timed.erase(timed.cbegin());
                    total_timed.erase(total_timed.cbegin());
                    next.erase(std::find(next.cbegin(), next.cend(), it));
                }
#ifdef __cpp_lib_coroutines
                else if (it == res_timed)
                {
                    if (auto const &res{timed_res[0](elapsed, total_res_timed[0])};
                        res.get_next() == true)
                    {
                        timed_res.erase(timed_res.cbegin());
                        total_res_timed.erase(total_res_timed.cbegin());
                        next.erase(std::find(next.cbegin(), next.cend(), it));
                    }
                }
#endif
                else if (it == block_loop)
                    loop[0](elapsed);
#ifdef __cpp_lib_coroutines
                else if (it == res_loop)
                    loop_res[0](elapsed);
#endif
            }
        }

    private:
        enum action_type
        {
#ifdef __cpp_lib_coroutines
            block_timed = 0,
            res_timed,
            block_loop,
            res_loop
#else
            block_timed = 0,
            block_loop
#endif
        };

        // raw memory
        std::byte timed_memory[10 * sizeof(timed_action_fn)];
        std::byte loop_memory[10 * sizeof(loop_action_fn)];
#ifdef __cpp_lib_coroutines
        std::byte res_timed_memory[10 * sizeof(resumable_timed_action_fn)];
        std::byte res_loop_memory[10 * sizeof(resumable_loop_action_fn)];
#endif

        /// buffers
        std::pmr::monotonic_buffer_resource timed_buffer{timed_memory, sizeof(timed_memory)};
        std::pmr::monotonic_buffer_resource loop_buffer{loop_memory, sizeof(loop_memory)};
#ifdef __cpp_lib_coroutines
        std::pmr::monotonic_buffer_resource res_timed_buffer{res_timed_memory, sizeof(res_timed_memory)};
        std::pmr::monotonic_buffer_resource res_loop_buffer{res_loop_memory, sizeof(res_loop_memory)};
#endif

        // detail vectors
        std::vector<action_type> next;
        std::vector<float> total_timed;
#ifdef __cpp_lib_coroutines
        std::vector<float> total_res_timed;
#endif

        // actions vectors
        std::pmr::vector<void (*)(float, float)> timed{&timed_buffer};
        std::pmr::vector<void (*)(float)> loop{&loop_buffer};
#ifdef __cpp_lib_coroutines
        std::pmr::vector<action_result (*)(float, float)> timed_res{&res_timed_buffer};
        std::pmr::vector<action_result (*)(float)> loop_res{&res_loop_buffer};
#endif
    };
} // namespace tnt

#endif //!TNT_ACTIONS_V2_IMPL_HPP
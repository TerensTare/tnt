#ifndef TNT_TYPE_UTILS_HPP
#define TNT_TYPE_UTILS_HPP

// TODO:
// separate multithreading utilities into a different file.
// TODO: check this
// https://www.youtube.com/watch?v=Zcqwb3CWqs4&pbjreload=10

#include <exception>
#include <memory>
#include <thread>
#include <system_error>

namespace tnt
{
    // based on
    // https://stackoverflow.com/questions/14650885/how-to-create-timer-events-using-c-11
    class run_after
    {
    public:
        template <class Callable, class... Args>
        run_after(int after, bool is_async, Callable &&f, Args &&... args)
        {
            std::function<typename std::result_of<Callable(Args...)>::type()>
                task(std::bind(std::forward<Callable>(f),
                               std::forward<Args>(args)...));
            if (is_async)
            {
                std::thread([after, task]() {
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(after));
                    task();
                }).detach();
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                task();
            }
        }
    };

    // TODO:
    // concepts usage (std::invocable, etc)
    template <typename... Ts>
    struct overload final : Ts...
    {
        using Ts::operator()...;
    };

    template <typename... Ts>
    overload(Ts...)->overload<Ts...>;

    // TODO: use boolean concept.
    // TODO: ensure ret can multiply an int
    inline constexpr auto if_then = [](bool cond, auto const &ret) {
        return (ret * cond);
    };

    inline constexpr auto if_else = [](bool cond, auto const &r1, auto const &r2) {
        static_assert(std::is_same_v<decltype(r1), decltype(r2)> ||
                          std::is_convertible_v<decltype(r1), decltype(r2)> ||
                          std::is_convertible_v<decltype(r2), decltype(r1)>,
                      "Parameters passed to if_else cannot be converted to each-other's type!!");
        return (r1 * cond + r2 * (!cond));
    };

    struct non_copyable
    {
        non_copyable() = default;
        non_copyable(non_copyable const &) = delete;
        non_copyable &operator=(non_copyable const &) = delete;
    };

    struct non_movable
    {
        non_movable() = default;
        non_movable(non_movable const &) = delete;
        non_movable &operator=(non_movable &&) = delete;
    };

    template <typename T, typename Deleter = std::default_delete<T>>
    struct singleton : non_copyable
    {
        ~singleton() noexcept(noexcept(Deleter())) { Deleter(); }

        static T &This() noexcept(std::is_nothrow_constructible_v<T>)
        {
            static T inst;
            return inst;
        }
    };

    // TODO: remove this when std::jthread is available.
    class scoped_thread : public non_copyable
    {
    public:
        explicit scoped_thread(std::thread &th) : t{std::move(th)}
        {
            if (!t.joinable())
                throw std::logic_error{"scoped_thread: No thread!!"};
        }

        ~scoped_thread() { t.join(); }

    private:
        std::thread t;
    };

    // thx Jonathan Boccara
    // https://www.fluentcpp.com/2017/05/19/crtp-helper/
    template <typename T>
    struct crtp
    {
        inline T &base() noexcept { return static_cast<T &>(*this); }
        inline T const &base() const noexcept { return static_cast<T const &>(*this); }
    };

    // thx Jonathan Boccara
    // https://www.fluentcpp.com/2020/05/29/how-to-make-derived-classes-implement-more-than-assignment/
    template <template <typename...> class Expression, typename Attempt, typename... Ts>
    struct is_detected : std::false_type
    {
    };

    template <template <typename...> class Expression, typename... Ts>
    struct is_detected<Expression, std::void_t<Expression<Ts...>>, Ts...> : std::true_type
    {
    };

    template <template <typename...> class Expression, typename... Ts>
    inline constexpr bool is_detected_v = is_detected<Expression, void, Ts...>::value;

} // namespace tnt

#endif //! TNT_TYPE_UTILS_HPP
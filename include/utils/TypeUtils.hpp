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
    struct overload : Ts...
    {
        using Ts::operator()...;
    };

    template <typename... Ts>
    overload(Ts...)->overload<Ts...>;

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

    template <typename T>
    struct Visitor
    {
        virtual void Visit(T &) = 0;
    };

    template <typename T, typename R = void>
    struct Visitor
    {
        typedef R ReturnType;
        virtual ReturnType Visit(T &) = 0;
    };

    template <typename R = void>
    struct Visitable
    {
        typedef R ReturnType;
        virtual ~Visitable() {}
        virtual ReturnType Accept(Visitor &) = 0;

    protected:
        template <typename T>
        static ReturnType AcceptImpl(T &visited, Visitor &guest)
        {
            if (Visitor<T> * p{dynamic_cast<Visitor<T> *>(&guest)})
                return p->Visit(visited);
            return ReturnType();
        }
    };

    // simple sample
    // class SomeVisitor :
    //  public BaseVisitor // required
    //  public Visitor<RasterBitmap>,
    //  public Visitor<Paragraph>
    // {
    // public:
    //  void Visit(RasterBitmap&); // visit a RasterBitmap
    //  void Visit(Paragraph &); // visit a Paragraph
    // };

    template <typename T, typename U>
    class is_convertible
    {
        typedef char Small;
        class Big
        {
            char dummy[2];
        };
        static Small Test(U);
        static Big Test(...);
        static T MakeT();

    public:
        enum
        {
            value = (sizeof(Test(MakeT())) == sizeof(Small))
        };
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
    inline constexpr bool is_detected_v = typename is_detected<Expression, void, Ts...>::value;

} // namespace tnt

#endif //! TNT_TYPE_UTILS_HPP
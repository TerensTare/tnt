// TODO:
// separate multithreading utilities into a different file.
// TODO: check this
// https://www.youtube.com/watch?v=Zcqwb3CWqs4&pbjreload=10

module;
#include <exception>
#include <memory>
#include <mutex>
#include <thread>
export module TnT.Utils : Types;

// TODO(maybe): lock should be std::unique_lock<std::recursive_mutex> ??
#define synchronized(mtx) for (std::unique_lock lock{mtx}; lock; lock.unlock())

namespace tnt
{
    // code taken from
    // https://stackoverflow.com/questions/14931982/synchronize-entire-class-in-c11
    template <typename T>
    struct Synchronized
    {
        explicit Synchronized(T &value) : t{value} {}

        Synchronized(Synchronized const &) = delete;
        Synchronized &operator=(Synchronized const &) = delete;
        Synchronized(Synchronized &&) = delete;
        Synchronized &operator=(Synchronized &&) = delete;

        template <typename Func>
        auto operator()(Func &&f) const -> decltype(f(t))
        {
            std::lock_guard guard{mtx};
            return f(t);
        }

    private:
        mutable T &t;
        mutable std::mutex mtx;
    };

    template <typenme T>
    Synchronized(T &)->Synchronized<T>;

    // template <typename To, auto Data, typename
    // std::enable_if<std::is_convertible_v<decltype(Data), To>, int> = 0>
    // struct convert
    // {
    //     enum
    //     {
    //         value = static_cast<To>(Data)
    //     };
    // };

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

        inline T &This() noexcept(noexcept(T{}))
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
                throw std::logic_error("scoped_thread: No thread!!");
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
} // namespace tnt

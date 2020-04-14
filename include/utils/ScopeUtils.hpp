#ifndef TNT_SCOPE_UTILS_HPP
#define TNT_SCOPE_UTILS_HPP

#include <exception>

namespace tnt
{
    // call the destructor of the object when it goes out of scope.
    // template <typename Constructor, typename Destructor>
    // class scope_guard
    // {
    // public:
    //     template <typename... Args>
    //     scope_guard(Args &&...args):data{} {}

    //     ~scope_guard()
    //     {
    //         data->~T();
    //     }

    //     scope_guard(scope_guard const &guard) : data{guard.data} {}
    //     scope_guard(scope_guard &&guard) : data{std::move(guard.data)} {}
    //     scope_guard &operator=(scope_guard const &) = delete;

    // private:
    //     T *data;
    // };

    // executes only if no exception is thrown in scope.
    template <typename F> class scope_success
    {
      public:
        explicit scope_success(const F &_f) : f{_f} {}
        scope_success(const scope_success &) = delete;
        scope_success &operator=(const scope_success &) = delete;

        ~scope_success() noexcept(noexcept(f()))
        {
            // f() might throw, as it can be caught normally.
            if (uncaught_exception_count == std::uncaught_exceptions())
                f();
        }

      private:
        F f;
        int uncaught_exception_count = std::uncaught_exceptions();
    };

    // executes only if any exception is thrown in scope.
    template <typename F> class scope_fail
    {
      public:
        explicit scope_fail(const F &_f) : f{_f} {}
        scope_fail(const scope_fail &) = delete;
        scope_fail &operator=(const scope_fail &) = delete;

        ~scope_fail()
        {
            // f() should not throw, else std::terminate is called.
            if (uncaught_exception_count != std::uncaught_exceptions())
                f();
        }

      private:
        F f;
        int uncaught_exception_count = std::uncaught_exceptions();
    };

    // execute function when goes out of scope.
    template <typename Function> class finally final
    {
      public:
        explicit finally(Function _f) : f{std::move(_f)} {}
        finally(const finally &) = delete;
        finally(finally &&)      = delete;

        finally &operator=(const finally &) = delete;
        finally &operator=(finally &&) = delete;

        ~finally() { f(); }

      private:
        Function f;
    };

    // call this.
    template <typename Function, typename = typename std::enable_if<
                                     std::is_function<Function>::value>::value>
    auto on_exit(Function &&f)
    {
        return finally<std::decay_t<Function>>{std::forward<Function>(f)};
    }
} // namespace tnt

#endif //! TNT_SCOPE_UTILS_HPP
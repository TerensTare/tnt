module;
#include <exception>
export module TnT.Utils : Scope;

namespace detail
{
  template <typename T>
  concept callable = std::is_function_v<T>;
}

export namespace tnt
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

  /// @brief Execute function only if no exception is thrown in the scope.
  /// @tparam F The type of the function to be wrapped by scope_success.
  template <typename F>
  class scope_success
  {
  public:
    /// @brief Create a new scope_success.
    /// @param f_ The function to be wrapped by the scope_success.
    ///@note f_() might throw, as it can be caught normally.
    explicit scope_success(const F &f_) : f{f_} {}

    scope_success(const scope_success &) = delete;
    scope_success &operator=(const scope_success &) = delete;

    /// @brief Destroy the scope_success.
    ~scope_success() noexcept(noexcept(f()))
    {
      if (uncaught_exception_count == std::uncaught_exceptions())
        f();
    }

  private:
    F f;
    int uncaught_exception_count = std::uncaught_exceptions();
  };

  /// @brief Execute function only if any exception is thrown in the scope.
  /// @tparam F The type of the function to be wrapped by scope_fail.
  template <typename F>
  class scope_fail
  {
  public:
    /// @brief Create a new scope_fail.
    /// @param f_ The function to be wrapped by the scope_fail.
    /// @note f_() should not throw, else @em std::terminate is called.
    explicit scope_fail(const F &f_) : f{f_} {}
    scope_fail(const scope_fail &) = delete;
    scope_fail &operator=(const scope_fail &) = delete;

    /// @brief Destroy the scope_fail.
    ~scope_fail()
    {
      if (uncaught_exception_count != std::uncaught_exceptions())
        f();
    }

  private:
    F f;
    int uncaught_exception_count = std::uncaught_exceptions();
  };

  /// @brief execute function when goes out of scope.
  /// @tparam Function The type of the function to be wrapped by finally.
  template <typename Function>
  class finally final
  {
  public:
    /// @brief Create a finally instance that wraps f_.
    /// @param f_ The function to be called by finally.
    explicit finally(Function &&f_) : f{std::move(f_)} {}

    finally(const finally &) = delete;
    finally(finally &&) = delete;

    finally &operator=(const finally &) = delete;
    finally &operator=(finally &&) = delete;

    /// @brief Destroy the finally instance.
    ~finally() noexcept(noexcept(f())) { f(); }

  private:
    Function f;
  };

  /// @brief Run a function when exiting the current scope.
  /// @param f A callable object.
  auto on_exit(detail::callable auto &&f)
  {
    return finally<std::decay_t<decltype(f)>>{std::forward<decltype(f)>(f)};
  }
} // namespace tnt

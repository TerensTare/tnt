#ifndef EXPECTED_HPP
#define EXPECTED_HPP

#include <exception>
#include <new>
#include <stdexcept>

// TODO: add comparison operators for two Excepted<T>.

namespace tnt
{
struct invalid
{
};

// code taken from
// https://bell0bytes.eu/expected/
template <class T>
class Expected
{
protected:
    union {
        T result;
        std::exception_ptr spam;
    };
    bool gotResult;
    Expected() {}

public:
    explicit Expected(T const &r) : result{r}, gotResult{true} {}
    explicit Expected(T &&r) : result{std::move(r)}, gotResult{true} {}

    Expected(Expected const &e) : gotResult{e.gotResult}
    {
        if (gotResult)
            new (&result) T{e.result};
        else
            new (&spam) std::exception_ptr{e.spam};
    }

    Expected(Expected &&e) : gotResult{e.gotResult}
    {
        if (gotResult)
            new (&result) T{std::move(e.result)};
        else
            new (&spam) std::exception_ptr{std::move(e.spam)};
    }

    template <typename E>
    explicit Expected<T>(E const &e)
        : spam{std::make_exception_ptr(e)}, gotResult{false} {}

    inline void swap(Expected const &e) noexcept
    {
        if (gotResult)
        {
            if (e.gotResult)
                std::swap(result, e.result);
            else
            {
                auto t{std::move(e.spam)};
                new (&e.result) T{std::move(result)};
                new (&spam) std::exception_ptr{t};
                std::swap(gotResult, e.gotResult);
            }
        }
        else
        {
            if (e.gotResult)
                e.swap(*this);
            else
                spam.swap(e.spam);
            std::swap(gotResult, rhs.gotResult);
        }
    }

    template <class E>
    static Expected<T> fromException(E const &e)
    {
        if (typeid(e) != typeid(E))
            throw std::invalid_argument("slicing detected!\n");
        return fromException(std::make_exception_ptr(e));
    }

    inline static Expected<T> fromException(std::exception_ptr const &p) noexcept
    {
        Expected<T> e;
        e.gotResult = false;
        new (&e.spam) std::exception_ptr{std::move(p)};
        return e;
    }

    inline static Expected<T> fromException()
    {
        return fromException(std::current_exception());
    }

    template <class F>
    static Expected fromCode(F func)
    try
    {
        return Expected{func()};
    }
    catch (...)
    {
        return fromException();
    }

    inline Expected &operator=(Expected const &e) noexcept
    {
        gotResult = e.gotResult;
        if (gotResult)
            new (&result) T{e.result};
        else
            new (&spam) std::exception_ptr{e.spam};
        return *this;
    }

    inline bool operator==(invalid const &) const noexcept { return !gotResult; }
    inline bool operator!=(invalid const &) const noexcept { return gotResult; }

    bool operator<=(invalid const &) const = delete;
    bool operator>=(invalid const &) const = delete;

    bool operator<(invalid const &) const = delete;
    bool operator>(invalid const &) const = delete;

    inline bool isValid() const noexcept { return gotResult; }
    inline bool successful() const noexcept { return gotResult; }

    inline T &get() noexcept(noexcept(gotResult))
    {
        if (!gotResult)
            std::rethrow_exception(spam);
        return result;
    }

    inline const T &get() const noexcept(noexcept(gotResult))
    {
        if (!gotResult)
            std::rethrow_exception(spam);
        return result;
    }

    template <class E>
    bool hasException() const
    {
        try
        {
            if (!gotResult)
                std::rethrow_exception(spam);
        }
        catch (const E &object)
        {
            (void)object;
            return true;
        }
        catch (...)
        {
        }
        return false;
    }

    friend class Expected<void>;
};

template <>
class Expected<void>
{
    std::exception_ptr spam;

public:
    template <typename E>
    explicit Expected(E const &e)
        : spam{std::make_exception_ptr(e)} {}

    template <typename T>
    explicit Expected(const Expected<T> &e)
    {
        if (!e.gotResult)
            new (&spam) std::exception_ptr{e.spam};
    }

    Expected(Expected &&o) : spam{std::move(o.spam)} {}
    Expected() : spam{} {}

    inline Expected &operator=(const Expected &e) noexcept
    {
        if (!e.isValid())
            this->spam = e.spam;
        return *this;
    }

    inline bool operator==(invalid const &) const noexcept { return static_cast<bool>(spam); }
    inline bool operator!=(invalid const &) const noexcept { return !spam; }

    inline bool isValid() const noexcept { return !spam; }
    inline bool wasSuccessful() const noexcept { return !spam; }

    inline void get() const noexcept(noexcept(isValid()))
    {
        if (!isValid())
            std::rethrow_exception(spam);
    }

    inline void suppress() noexcept {}
};

template <typename T>
Expected(T)->Expected<T>;

template <typename T>
Expected(T[])->Expected<T[]>;

template <typename... Args>
Expected(Args &&...)->Expected<std::tuple<Args...>>;
} // namespace tnt

#endif //! EXPECTED_HPP
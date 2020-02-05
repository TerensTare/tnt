#ifndef EXPECTED_HPP
#define EXPECTED_HPP

#include <atomic>
#include <exception>
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
    Expected(T const &r) : result{r}, gotResult{true} {}
    Expected(T &&r) : result{std::move(r)}, gotResult{true} {}

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
    Expected<T>(E const &e) : spam{std::make_exception_ptr(e)}, gotResult{false} {}

    void swap(Expected &e)
    {
        if (gotResult)
        {
            if (e.gotResult)
                std::swap(result, e.result);
            else
            {
                auto t{std::move(e.spam)};
                new (&e.result) T{std::move(result)};
                new (&spam) std::exception_ptr;
                std::swap(gotResult, e.gotResult);
            }
        }
        else
        {
            if (e.gotResult)
                e.swap(*this);
            else
                spam.swap(e.spam);
        }
    }

    template <class E>
    static Expected<T> fromException(E const &e)
    {
        if (typeid(e) != typeid(E))
            throw std::invalid_argument("slicing detected!\n");
        return fromException(std::make_exception_ptr(e));
    }

    static Expected<T> fromException(std::exception_ptr p)
    {
        Expected<T> e;
        e.gotResult = false;
        new (&e.spam) std::exception_ptr(std::move(p));
        return e;
    }

    static Expected<T> fromException()
    {
        return fromException(std::current_exception());
    }

    Expected &operator=(Expected const &e)
    {
        gotResult = e.gotResult;
        if (gotResult)
            new (&result) T{e.result};
        else
            new (&spam) std::exception_ptr{e.spam};
        return *this;
    }

    bool operator==(invalid) const { return !gotResult; }
    bool operator!=(invalid) const { return gotResult; }

    bool operator<=(invalid) const = delete;
    bool operator>=(invalid) const = delete;

    bool operator<(invalid) const = delete;
    bool operator>(invalid) const = delete;

    bool isValid() const { return gotResult; }
    bool successful() const { return gotResult; }

    T &get()
    {
        if (!gotResult)
            std::rethrow_exception(spam);
        return result;
    }

    const T &get() const
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
    Expected(E const &e) : spam{std::make_exception_ptr(e)} {}

    template <typename T>
    Expected(const Expected<T> &e)
    {
        if (!e.gotResult)
            new (&spam) std::exception_ptr{e.spam};
    }

    Expected(Expected &&o) : spam{std::move(o.spam)} {}
    Expected() : spam{} {}

    Expected &operator=(const Expected &e)
    {
        if (!e.isValid())
            this->spam = e.spam;
        return *this;
    }

    bool operator==(invalid) const { return bool(spam); }
    bool operator!=(invalid) const { return !spam; }

    bool isValid() const { return !spam; }
    bool wasSuccessful() const { return !spam; }

    void get() const
    {
        if (!isValid())
            std::rethrow_exception(spam);
    }

    void suppress() {}
};

} // namespace tnt

#endif //!EXPECTED_HPP
#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

namespace tnt
{
template <typename T>
concept reversable = requires(T t)
{
    t.rbegin;
    t.rend;
};

template <typename T>
concept renderable = requires(T t)
{
    t.Draw;
};

template <typename T>
concept observable = requires(T t)
{
    t.Attach;
    t.Detach;
    t.Notify;
};

template <typename T>
concept singleton = requires
{
    {
        T::This()
    }
    ->T &;
};
} // namespace tnt

#endif //!CONCEPTS_HPP
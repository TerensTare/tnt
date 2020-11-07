#ifndef TNT_CONTAINERS_HPP
#define TNT_CONTAINERS_HPP

#include <mutex>
#include <queue>

#include <nlohmann/json_fwd.hpp>

namespace tnt
{
    template <class T>
    class circular_buffer
    {
    public:
        explicit circular_buffer(size_t _size)
            : buffer{std::unique_ptr<T[]>{new T[_size]}}, max{_size}
        {
        }

        inline void add(T item) noexcept
        {
            std::lock_guard<std::mutex> lock{mtx};

            buffer[head] = item;

            if (isFull)
                tail = (tail + 1) % max;
            head = (head + 1) % max;

            isFull = (head == tail);
        }

        inline T get() noexcept
        {
            std::lock_guard<std::mutex> lock{mtx};

            if (empty())
                return T();
            auto val{buffer[tail]};
            isFull = false;
            tail = (tail + 1) % max;

            return val;
        }

        inline void reset() noexcept
        {
            std::lock_guard<std::mutex> lock{mtx};
            head = tail;
            isFull = false;
        }

        inline bool empty() const noexcept { return (!isFull && (head == tail)); }

        inline bool full() const noexcept { return isFull; }

        inline size_t capacity() const noexcept { return max; }

        inline size_t size() const noexcept
        {
            size_t s{max};

            if (!isFull)
                if (head >= tail)
                    s = head - tail;
                else
                    s = max + head - tail;
            return s;
        }

    private:
        size_t head{0};
        size_t tail{0};
        const size_t max;
        bool isFull{false};

        std::mutex mtx;
        std::unique_ptr<T[]> buffer;
    };

    // based on
    // https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
    template <typename T>
    class concurrent_queue
    {
    public:
        inline void push(T const &item)
        {
            std::scoped_lock lock{mutex};
            data.push(item);
            lock.unlock();
            cv.notify_one();
        }

        inline bool empty() const noexcept
        {
            std::scoped_lock lock{mutex};
            return data.empty();
        }

        inline bool try_pop(T &value) noexcept(noexcept(data.front() && data.pop()))
        {
            std::scoped_lock lock{mutex};
            if (data.empty())
                return false;
            value = data.front();
            data.pop();
            return true;
        }

        inline void wait_and_pop(T &value)
        {
            std::scoped_lock lock{mutex};
            while (data.empty())
                cv.wait(lock);
            value = data.front();
            data.pop();
        }

    private:
        std::condition_variable cv;
        std::queue<T> data;
        mutable std::mutex mutex;
    };

    namespace detail
    {
        // clang-format off
        template <typename T>
        concept has_contains = std::ranges::input_range<T>
            && requires(T const &t)
        {
            { t.contains } -> std::same_as<bool>;
            requires std::invocable<
                decltype(t.contains),
                std::ranges::range_value_t<T>>;
        };

        template <typename T>
        concept has_key = std::ranges::input_range<T> &&
            requires { typename T::key_type; };
        // clang-format on
    } // namespace detail

    /// @brief Check if the range with the given begin/end contains a certain value.
    /// @tparam R The type of the range.
    /// @tparam T The type of the element to search.
    /// @param rng The range to check.
    /// @param value The desired value to find.
    /// @return bool
    template <std::ranges::input_range R,
              typename T = std::ranges::range_value_t<R>>
    constexpr bool contains(R const &rng, std::remove_cvref_t<T> const &value)
    {
        if constexpr (detail::has_contains<R>)
            return r.contains(value);
        return !(std::ranges::empty(rng) ||
                 std::ranges::find(rng, value) ==
                     std::ranges::end(rng));
    }

    template <typename R, typename... Ts>
    inline const auto matches(const R &range, Ts... ts) noexcept
    {
        return (std::count(std::begin(range), std::end(range), ts) + ...);
    }

    // thx Tristan Brindle
    // https://tristanbrindle.com/posts/beware-copies-initializer-list
    template <typename T, typename... Args>
    std::vector<T> make_vector(Args &&... args)
    {
        std::vector<T> vec;
        vec.reserve(sizeof...(Args));
        (vec.emplace_back(std::forward<Args>(args)), ...);
        return vec;
    }

    // ! Works only with std::set types.
    template <typename T, typename... Ts>
    inline bool set_insert_all(T &set, Ts... ts)
    {
        return (set.insert(ts).second && ...);
    }

    // ! Works only with std::vector types.
    template <typename T, typename... Ts>
    inline void vector_insert_all(std::vector<T> &vec, Ts... ts)
    {
        (vec.push_back(ts), ...);
    }

    template <typename T>
    inline void quick_remove_at(std::vector<T> &v, std::size_t idx) noexcept
    {
        if (idx < v.size())
        {
            v[idx] = std::move(v.back());
            v.pop_back();
        }
    }

    template <typename T>
    inline void quick_remove_at(
        std::vector<T> &v,
        typename std::vector<T>::iterator it) noexcept
    {
        if (it != std::end(v))
        {
            *it = std::move(v.back());
            v.pop_back();
        }
    }

    template <typename C, typename T>
    void insert_sorted(C &v, const T &item)
    {
        auto const &insert_pos{
            std::lower_bound(std::begin(v), std::end(v), item)};
        v.insert(insert_pos, item);
    }

    template <typename T, typename... Ts>
    inline bool within(T min, T max, Ts... ts) noexcept
    {
        return ((min <= ts && ts <= max) && ...);
    }
} // namespace tnt

#endif //!TNT_CONTAINERS_HPP
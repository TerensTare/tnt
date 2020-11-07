#ifndef TNT_SPARSE_SET_HPP
#define TNT_SPARSE_SET_HPP

#include <compare>
#include <vector>

#include "utils/Assert.hpp"

// thx skypjack
// https://skypjack.github.io/2020-08-02-ecs-baf-part-9/

// TODO:
// document/test this.
// constexpr.
// recheck operator <=>.

namespace tnt
{
    template <std::unsigned_integral>
    class sparse_iterator;

    inline constexpr struct as_sentinel_t final
    {
    } as_sentinel;

    struct sparse_sentinel final
    {
        template <std::unsigned_integral T>
        inline bool operator==(sparse_iterator<T> const &rhs) const noexcept
        {
            return (rhs.index < 0 || rhs.index >= rhs.owner->size());
        }

        template <std::unsigned_integral T>
        operator sparse_iterator<T>() noexcept;
    };

    template <std::unsigned_integral T>
    class sparse_set
    {
        inline static constexpr std::size_t null{(std::size_t)-1};

    public:
        using value_type = T;
        using reference = value_type &;
        using const_reference = const value_type &;

        using iterator = sparse_iterator<T>;
        using const_iterator = sparse_iterator<T>;
        using sentinel = sparse_sentinel;

        /// @brief Create a new empty sparse set.
        sparse_set() = default;

        sparse_set(sparse_set<T> const &) = default;
        sparse_set &operator=(sparse_set<T> const &) = default;

        template <typename U>
        sparse_set(sparse_set<U> const &) = delete;
        template <typename U>
        sparse_set &operator=(sparse_set<U> const &) = delete;

        /// @brief Return the number of the elements stored on the sparse set.
        /// @return std::size_t
        [[nodiscard]] inline std::size_t size() const noexcept { return size_; }

        /// @brief Check if the sparse set is empty or not.
        /// @return bool
        [[nodiscard]] inline bool empty() const noexcept { return (size_ == 0); }

        /// @brief Set the size of the container to 0.
        inline void clear() noexcept { size_ = 0; }

        /// @brief Check if the set contains a specific element.
        /// @param val The value to check.
        /// @return bool
        [[nodiscard]] inline bool contains(value_type const val) const noexcept
        {
            return (val < sparse.size() && sparse[val] != null);
        }

        /// @brief Return the index of the given value.
        /// @param val The desired value.
        /// @return std::size_t
        [[nodiscard]] inline std::size_t index(value_type const val)
        {
            return sparse[val];
        }

        /// @brief Insert val at the end of the container.
        /// @param val The value to insert.
        inline void push_back(value_type const val)
        {
            insert(size_, val);
        }

        /// @brief Remove the last element of the sparse set.
        inline void pop_back()
        {
            safe_ensure(!empty(), "Calling pop_back() on empty sparse_set!!");

            sparse[dense[--size_]] = null;
        }

        /// @brief Remove the given element from the sparse set.
        /// @param val The value to be removed.
        inline void erase(value_type const val)
        {
            safe_ensure(contains(val), "Erasing non-existent value from sparse_set!!");

            value_type const &last{dense[size_ - 1]};
            std::swap(dense[size_ - 1], dense[sparse[val]]);
            sparse[val] = null;
            dense.erase(dense.cbegin() + --size_);
        }

        /// @brief Add a new value at the given index.
        /// @param index Where to add the value.
        /// @param val The desired value.
        inline void insert(std::size_t const index, value_type const val)
        {
            if (val == (T)null && index < size_ &&
                contains(dense[index]))
                erase(dense[index]);
            else
            {
                if (val == size_)
                    ++size_;

                sparse.insert(sparse.cbegin() + val,
                              *dense.insert(dense.cbegin() + index, val));
            }
        }

        /// @overload
        /// @brief Add a new value before the given iterator.
        /// @param it Where to add the value.
        /// @param val The desired value.
        inline void insert(sparse_iterator<T> it, value_type const val)
        {
            if (val == (T)null && contains(*it))
                erase(*it);
            else
            {
                if (it >= end())
                    ++size_;

                sparse.insert(sparse.cbegin() + val,
                              *dense.insert(dense.cbegin() + *it, val));
            }
        }

        /// @brief Change the value at the given index.
        /// @param index The desired index.
        /// @param val The new value.
        inline void edit(std::size_t const index, value_type const val)
        {
            safe_ensure(size_ > index && contains(dense[index]),
                        "Editing non-existant index of sparse_set!!");

            if (val == (T)null && index < size_ &&
                contains(dense[index]))
                erase(dense[index]);
            else
            {
                sparse[dense[index]] = null;
                dense[index] = val;
                sparse[val] = index;
            }
        }

        /// @brief Get the element at the given index.
        /// @param index The desired index to access.
        /// @return const_reference
        [[nodiscard]] inline const_reference at(std::size_t const index) const
        {
            safe_ensure(size_ > index && contains(dense[index]),
                        "Accessing invalid index of sparse_set!!");
            return dense.at(index);
        }

        /// @brief Get the element at the given index.
        /// @param index The desired index to access.
        /// @return const_reference
        [[nodiscard]] inline const_reference operator[](std::size_t const index) const noexcept
        {
            safe_ensure(size_ > index && contains(dense[index]),
                        "Accessing invalid index of sparse_set!!");
            return dense[index];
        }

        /// @brief Return the first element of the sparse set.
        /// @return const_reference
        [[nodiscard]] inline const_reference front() const noexcept
        {
            safe_ensure(!empty(), "Calling front() on empty sparse_set()!!");
            return dense[size_ - 1];
        }

        /// @brief Return the last element of the sparse set.
        /// @return const_reference
        [[nodiscard]] inline const_reference back() const noexcept
        {
            safe_ensure(!empty(), "Calling back() on empty sparse_set()!!");
            return dense[0];
        }

        inline bool operator==(sparse_set<T> const &other) const
        {
            return ((size_ == other.size()) && (dense == other.dense));
        }

        template <typename U>
        inline bool operator==(sparse_set<U> const &) const = delete;

        /// @brief Return an iterator to the first element of the sparse set.
        /// @return iterator.
        inline iterator begin() noexcept { return iterator{*this}; }

        /// @brief Return an const_iterator to the first element of the sparse set.
        /// @return const_iterator.
        inline const_iterator cbegin() const noexcept { return const_iterator{*this}; }

        /// @brief Return a sentinel that points to the end of the sparse_set.
        /// @return sentinel
        inline sentinel end() noexcept { return sparse_sentinel{}; }

        /// @brief Return a constant sentinel that points to the end of the sparse_set.
        /// @return sentinel
        inline sentinel cend() const noexcept { return sparse_sentinel{}; }

        /// @brief Return an iterator to the desired element, or end() if it is not found.
        /// @param val The value to be wrapped on an iterator.
        /// @return const_iterator
        [[nodiscard]] inline const_iterator find(value_type const val) const noexcept
        {
            if (contains(val))
                return const_iterator{*this} + sparse[val];
            return const_iterator{*this} + size_; // end()
        }

    private:
        std::vector<T> dense;
        std::vector<std::size_t> sparse;
        std::size_t size_{0};
    };

    template <std::unsigned_integral T>
    class sparse_iterator
    {
        inline sparse_iterator(sparse_set<T> const &owner_, std::size_t index_) noexcept
            : owner{&owner_}, index{index_} {}

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = const T;
        using iterator_category = std::random_access_iterator_tag;

        inline explicit sparse_iterator(sparse_set<T> const &owner_) noexcept
            : owner{&owner_}, index{owner_.size() - 1} {}

        inline sparse_iterator(sparse_set<T> const *owner_, as_sentinel_t) noexcept
            : owner{owner_}, index{(std::size_t)-1} {}

        sparse_iterator(sparse_iterator const &) = default;
        sparse_iterator(sparse_iterator &&) = default;

        sparse_iterator &operator=(sparse_iterator const &) = default;
        sparse_iterator &operator=(sparse_iterator &&) = default;

        template <typename U>
        sparse_iterator(sparse_iterator<U> const &) = delete;
        template <typename U>
        sparse_iterator(sparse_iterator<U> &&) = delete;

        template <typename U>
        sparse_iterator &operator=(sparse_iterator<U> const &) = delete;
        template <typename U>
        sparse_iterator &operator=(sparse_iterator<U> &&) = delete;

        inline sparse_iterator &operator=(sparse_sentinel)
        {
            index = (std::size_t)-1;
        }

        inline sparse_iterator &operator++()
        {
            --index;
            return *this;
        }

        inline sparse_iterator operator++(int)
        {
            return {*owner, index--};
        }

        inline sparse_iterator operator+(difference_type const rhs) const
        {
            return {*owner, index - rhs};
        }

        inline sparse_iterator &operator+=(difference_type const rhs)
        {
            index -= rhs;
            return *this;
        }

        inline sparse_iterator &operator--()
        {
            return sparse_iterator<T>{*owner, ++index};
        }

        inline sparse_iterator operator--(int)
        {
            return {*owner, index++};
        }

        inline sparse_iterator operator-(difference_type const rhs) const
        {
            return {*owner, index + rhs};
        }

        inline difference_type operator-(sparse_iterator const &rhs) const
        {
            return index - rhs.index;
        }

        inline sparse_iterator &operator-=(difference_type const rhs)
        {
            return {*owner, index += rhs};
        }

        inline value_type &operator*() const
        {
            safe_ensure(owner, "Dereferencing iterator of invalid sparse_set!!");
            return owner->at(index);
        }

        inline value_type &operator[](difference_type const diff)
        {
            safe_ensure(owner, "Dereferencing iterator of invalid sparse_set!!");
            return owner->at(index - diff);
        }

        inline bool operator==(sparse_iterator<T> const &rhs) const noexcept
        {
            return (index == rhs.index);
        }

        inline bool operator==(sparse_sentinel const &) const noexcept
        {
            return (index >= owner->size() || index < 0);
        }

        inline auto operator<=>(sparse_iterator const &rhs) const noexcept
        {
            return (*this - rhs) <=> 0;
        }

    private:
        const sparse_set<T> *owner{nullptr};
        std::size_t index{0};
    };

    template <std::unsigned_integral T>
    sparse_iterator(sparse_set<T> const *) noexcept->sparse_iterator<T>;

    template <std::unsigned_integral T>
    sparse_iterator(sparse_set<T> const *, as_sentinel_t) noexcept->sparse_iterator<T>;

    template <std::unsigned_integral T>
    inline sparse_sentinel::operator sparse_iterator<T>() noexcept
    {
        return sparse_iterator<T>{nullptr, as_sentinel};
    }

    template <typename T, typename U>
    bool operator==(sparse_iterator<T>, sparse_iterator<U>) = delete;

    template <std::unsigned_integral T>
    inline sparse_iterator<T> operator+(
        typename sparse_iterator<T>::difference_type const diff,
        sparse_iterator<T> const &it)
    {
        return {it.owner, it.index - diff};
    }

    template <typename T, typename U>
    auto operator<=>(sparse_iterator<T>, sparse_iterator<U>) = delete;

    template <std::unsigned_integral T>
    inline typename sparse_set<T>::iterator begin(sparse_set<T> const &sparse) noexcept
    {
        return sparse.begin();
    }

    template <std::unsigned_integral T>
    inline typename sparse_set<T>::const_iterator cbegin(sparse_set<T> const &sparse) noexcept
    {
        return sparse.cbegin();
    }

    // TODO: sentinel here
    template <std::unsigned_integral T>
    inline typename sparse_set<T>::sentinel end(sparse_set<T> const &sparse) noexcept
    {
        return sparse.end();
    }

    template <std::unsigned_integral T>
    inline typename sparse_set<T>::sentinel cend(sparse_set<T> const &sparse) noexcept
    {
        return sparse.cend();
    }
} // namespace tnt

#endif // !TNT_SPARSE_SET_HPP
#ifndef TNT_SPARSE_SET_HPP
#define TNT_SPARSE_SET_HPP

#include <compare>
#include <vector>

#include "utils/Assert.hpp"

// thx skypjack
// https://skypjack.github.io/2020-08-02-ecs-baf-part-9/

// TODO:
// document/test this.
// handle "holes".
// constexpr.

// TODO(maybe):
// support more types (if so, add sparse_iterator::operator->)
// make sparse_iterator and sparse_sentinel a subclass of sparse_set ??

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
            return (rhs.index == -1 || rhs.index >= rhs.owner->size());
        }
    };

    template <std::unsigned_integral T>
    class sparse_set
    {
    public:
        using value_type = T;
        using reference = value_type &;
        using const_reference = const value_type &;

        using iterator = sparse_iterator<T>;
        using const_iterator = sparse_iterator<T>;
        using sentinel = sparse_sentinel;

        /// @brief Create a new empty sparse set.
        sparse_set() = default;

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
            return (sparse.size() < val && size_ > sparse[val] &&
                    dense[sparse[val]] == val);
        }

        /// @brief Return the index of the given

        /// @brief Insert val at the end of the container.
        /// @param val The value to insert.
        inline void push_back(value_type const val)
        {
            dense.push_back(val);
            if (sparse.capacity() < val)
                sparse.reserve(val - sparse.capacity());
            sparse.insert(sparse.cbegin() + val, size_++);
        }

        /// @brief Remove the last element of the sparse set.
        inline void pop_back()
        {
            safe_ensure(!empty(), "Calling pop_back() on empty sparse_set!!");
            erase(dense[size_ - 1]);
        }

        /// @brief Remove the given element from the sparse set.
        /// @param val The value to be removed.
        inline void erase(value_type const val)
        {
            safe_ensure(contains(val), "Erasing non-existent value from sparse_set!!");
            value_type const &last{dense.back()};
            std::swap(dense.back(), dense[sparse[val]]);
            std::swap(sparse[last], sparse[val]);
            dense.pop_back();
            --size_;
        }

        /// @brief Get the element at the given index.
        /// @param index The desired index to access.
        /// @return const_reference
        [[nodiscard]] inline const_reference at(std::size_t const index) const
        {
            safe_ensure(size_ > index,
                        "Accessing out-of-range index of sparse_set!!");
            return dense.at(index);
        }

        /// @brief Get the element at the given index.
        /// @param index The desired index to access.
        /// @return const_reference
        [[nodiscard]] inline const_reference operator[](std::size_t const index) const noexcept
        {
            safe_ensure(size_ > index,
                        "Accessing out-of-range index of sparse_set!!");
            return dense[index];
        }

        /// @brief Return the first element of the sparse set.
        /// @return const_reference
        [[nodiscard]] inline const_reference front() const noexcept
        {
            safe_ensure(!empty(), "Calling front() on empty sparse_set()!!");
            return dense[0];
        }

        /// @brief Return the last element of the sparse set.
        /// @return const_reference
        [[nodiscard]] inline const_reference back() const noexcept
        {
            safe_ensure(!empty(), "Calling back() on empty sparse_set()!!");
            return dense[size_ - 1];
        }

        /// @brief Return an iterator to the first element of the sparse set.
        /// @return iterator.
        inline iterator begin() noexcept { return iterator{this}; }

        /// @brief Return an const_iterator to the first element of the sparse set.
        /// @return const_iterator.
        inline const_iterator cbegin() const noexcept { return const_iterator{this}; }

        /// @brief Return a sentinel that points to the end of the sparse_set.
        /// @return sentinel
        inline sentinel end() noexcept { return sparse_sentinel{}; }

        /// @brief Return a constant sentinel that points to the end of the sparse_set.
        /// @return sentinel
        inline sentinel cend() const noexcept { return sparse_sentinel{}; }

        /// @brief Return an iterator to the desired element, or end() if it is not found.
        /// @param val The value to be wrapped on an iterator.
        /// @return const_iterator
        [[nodiscard]] inline const_iterator find(value_type const val) const
        {
            if (contains(val))
                return const_iterator{this, sparse[val]};
            return const_iterator{this};
        }

    private:
        std::vector<T> dense;
        std::vector<std::size_t> sparse;
        std::size_t size_{0};
    };

    // TODO: operator=.

    template <std::unsigned_integral T>
    class sparse_iterator
    {
        inline sparse_iterator(const sparse_set<T> *owner_, std::size_t index_)
            : owner{owner_}, index{index_} {}

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = const T;
        using iterator_category = std::random_access_iterator_tag;
        using iterator_concept = iterator_category;

        inline explicit sparse_iterator(sparse_set<T> const *owner_)
            : owner{owner_}, index{owner_->size()} {}

        inline sparse_iterator(sparse_set<T> const *owner_, as_sentinel_t)
            : owner{owner_}, index{-1} {}

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

        inline sparse_iterator &operator++()
        {
            --index;
            return *this;
        }

        inline sparse_iterator operator++(int)
        {
            return {owner, index--};
        }

        inline sparse_iterator operator+(difference_type const rhs) const
        {
            return {owner, index - rhs};
        }

        inline sparse_iterator &operator+=(difference_type const rhs)
        {
            return {owner, index -= rhs};
        }

        inline sparse_iterator &operator--()
        {
            ++index;
            return *this;
        }

        inline sparse_iterator operator--(int)
        {
            return {owner, index++};
        }

        inline sparse_iterator operator-(difference_type const rhs) const
        {
            return {owner, index + rhs};
        }

        inline difference_type operator-(sparse_iterator const &rhs) const
        {
            return index - rhs.index;
        }

        inline sparse_iterator &operator-=(difference_type const rhs)
        {
            return {owner, index += rhs};
        }

        inline value_type &operator*() const
        {
            safe_ensure(index != -1 && owner->size() > index,
                        "Dereferencing out-of-range sparse_iterator!!");
            return owner->at(index);
        }

        inline value_type &operator[](difference_type const diff)
        {
            safe_ensure(index != -1 && index > diff,
                        "Dereferencing out-of-range sparse_iterator!!");
            return owner->at(index - diff);
        }

        inline bool operator==(sparse_iterator<T> const &rhs) const noexcept
        {
            return (index == rhs.index);
        }

        inline bool operator==(sparse_sentinel const &) const noexcept
        {
            return (index >= owner->size());
        }

        inline auto operator<=>(sparse_iterator const &rhs) const noexcept
        {
            return *(*this - rhs) <=> 0;
        }

    private:
        const sparse_set<T> *owner{nullptr};
        std::size_t index{0};
    };

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
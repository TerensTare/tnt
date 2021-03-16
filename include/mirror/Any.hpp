#ifndef TNT_ANY_MIRROR_HPP
#define TNT_ANY_MIRROR_HPP

#include "mirror/TypeInfo.hpp"
#include "utils/Assert.hpp"

// TODO:
// constexpr/noexcept everything
// recheck noexcept
// recheck .to/any_cast to avoid temporaries
// support moving/copying storage of any-s with different buff size.
// https://discord.com/channels/707607951396962417/788669411560914954/790524716829245460
// operator bool()/empty
// operator sized_any<S2>/T
// handle cases where you assign an object with size bigger than the buff
// any_cast pointers

// TODO(maybe):
// support for C-style arrays ??
// redeclare dtor_fn to have a std::size_t as template parameter
// copy/move member fptr ??

namespace tnt
{
    template <typename>
    struct in_place_tag_t
    {
        explicit in_place_tag_t() = default;
    };

    template <typename T>
    inline constexpr in_place_tag_t<T> in_place_t{};

    template <std::size_t>
    struct size_tag_t
    {
        explicit size_tag_t() = default;
    };

    template <std::size_t S>
    inline constexpr size_tag_t<S> size_tag{};

    inline constexpr size_tag_t<(std::size_t)-1> deduce_size{};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail
    {
        template <typename T>
        struct is_sized_any : std::false_type
        {
        };

        template <typename T>
        concept not_any = not is_sized_any<std::remove_cvref_t<T>>::value;
    } // namespace detail
#endif

    template <std::size_t S>
    class sized_any final
    {
    public:
        constexpr sized_any() noexcept
            : buff{}, data{static_cast<void *>(&buff)}, dtor{nullptr}, info{tnt::type_id<void>()} {}

        template <std::size_t S2>
        constexpr sized_any(sized_any<S2> const &other) noexcept
            : buff{other.buff}, data{static_cast<void *>(&buff)}, info{other.info},
              dtor{[&other](sized_any &a) { other.dtor(a); }}
        {
            // if constexpr (S2 > S)
            // {
            //     dbuff = new std::byte[S2];
            //     data = static_cast<void *>(&dbuff);
            // }
            // data = other.data;
        }

        template <std::size_t S2>
        constexpr sized_any(sized_any<S2> &&other) noexcept
            : data{static_cast<void *>(&buff)},
              dtor{std::exchange(other.dtor, nullptr)},
              info{std::exchange(other.info, tnt::type_id<void>())}
        {
            // if constexpr (S2 > S)
            // {
            //     dbuff = new std::byte[S2];
            //     data = static_cast<void *>(&dbuff);
            // }
            data = std::exchange(other.data, nullptr);
        }

        template <detail::not_any T>
        constexpr sized_any(T &&t)
            : data{static_cast<void *>(&buff)},
              dtor{[](sized_any &a) { if(a.data)static_cast<T*>(a.data)->~T(); }},
              info{tnt::type_id<T>()}
        {
            new (data) T{std::move(t)};
        }

        template <detail::not_any T, typename... Args>
        requires std::constructible_from<T, Args...> explicit constexpr sized_any(tnt::in_place_tag_t<T>, Args &&... args) noexcept(
            std::is_nothrow_constructible_v<T, Args...>)
            : data{static_cast<void *>(&buff)}, info{tnt::type_id<T>()}
        {
            emplace<T>(std::forward<Args>(args)...);
        }

        template <std::size_t S2>
        constexpr sized_any &operator=(sized_any<S2> const &other)
        {
            sized_any{other}.swap(*this);
            return *this;
        }

        template <std::size_t S2>
        constexpr sized_any &operator=(sized_any<S2> &&other)
        {
            other.swap(*this);
            sized_any{}.swap(other);
            return *this;
        }

        template <typename T>
        constexpr sized_any &operator=(T &&t)
        {
            sized_any{std::forward<T>(t)}.swap(*this);
            return *this;
        }

        constexpr ~sized_any() noexcept
        {
            reset();

            // if (dbuff)
            // {
            //     delete[] dbuff;
            //     dbuff = nullptr;
            // }
        }

        template <std::size_t S2>
        constexpr sized_any &swap(sized_any<S2> &other) noexcept
        {
            // if constexpr (S2 > S)
            // {
            //     dbuff = new std::byte[S2];
            //     data = static_cast<void *>(&dbuff);
            // }

            {
                void *tmp{data};
                data = other.data;
                other.data = tmp;
            }
            {
                tnt::type_info const tmp{info};
                info = other.info;
                other.info = tmp;
            }

            return *this;
        }

        constexpr void reset() noexcept
        {
            if (dtor)
            {
                dtor(*this);
                dtor = nullptr;
            }
            data = nullptr;
            info = tnt::type_id<void>();
        }

        constexpr bool has_value() noexcept
        {
            return (info != tnt::type_id<void>());
        }

        explicit constexpr operator bool() noexcept { return (info != tnt::type_id<void>()); }

        constexpr const tnt::type_info &type() const noexcept { return info; }

        template <std::size_t Si>
        constexpr operator sized_any<Si>() noexcept
        {
            return {*this};
        }

        template <not_any T>
        constexpr operator T() const noexcept
        {
            safe_ensure(info == tnt::type_id<T>(), "Converting tnt::sized_any to wrong type!!");
            return to<T>();
        }

        // clang-format off
        template<typename T,typename ...Args>
        requires std::constructible_from<T, Args...>
        constexpr void emplace(Args&&...args) noexcept(
            std::is_nothrow_constructible_v<T, Args...>)
        {
            // clang-format on
            reset();
            data = static_cast<void *>(&buff);
            new (data) T{std::forward<Args>(args)...};
            info = tnt::type_id<T>();
            dtor = [](sized_any &a) { if(a.data)static_cast<T *>(a.data)->~T(); };
        }

        template <typename T>
        inline const T to() const noexcept
        {
            using U = const std::remove_cvref_t<T> &;
            static_assert(std::constructible_from<T, U>);
            safe_ensure(info == tnt::type_id<T>(),
                        "tnt::sized_any: Casting to wrong type!!");

            using R = std::remove_reference_t<T>;
            R *res{static_cast<R *>(data)};
            safe_ensure(res, "tnt::sized_any: Cast produced invalid data!!");

            if constexpr (std::is_reference_v<T>)
                return *res;
            else
                return static_cast<std::add_lvalue_reference_t<T>>(*res);
        }

        template <typename T>
        inline T to() noexcept
        {
            using U = std::remove_cvref_t<T> &;
            static_assert(std::constructible_from<T, U>);
            safe_ensure(info == tnt::type_id<T>(),
                        "tnt::sized_any: Casting to wrong type!!");

            using R = std::remove_reference_t<T>;
            R *res{static_cast<R *>(data)};
            safe_ensure(res, "tnt::sized_any: cast produced invalid data!!");

            if constexpr (std::is_reference_v<T>)
                return *res;
            else
                return static_cast<std::add_lvalue_reference_t<T>>(*res);
        }

    private:
        template <std::size_t Si>
        inline static constexpr std::size_t buff_size{Si};

        template <>
        inline static constexpr std::size_t buff_size<(std::size_t)-1>{64};

        using dtor_fn = void(sized_any &);

        void *data;
        dtor_fn *dtor{nullptr};
        tnt::type_info info;
        std::byte buff[buff_size<S>];

        // union
        // {
        //     std::byte *dbuff;
        //     std::byte buff[buff_size<S>];
        // };
    };

    namespace detail
    {
        template <std::size_t Si>
        struct is_sized_any<sized_any<Si>> : std::true_type
        {
        };
    } // namespace detail

    template <detail::not_any T>
    sized_any(T &&) -> sized_any<sizeof(T)>;

    template <detail::not_any T, typename... Args>
    sized_any(tnt::in_place_tag_t<T>, Args &&...) -> sized_any<sizeof(T)>;

    template <std::size_t S, detail::not_any T>
    sized_any(tnt::size_tag_t<S>, T &&) -> sized_any<S>;

    template <std::size_t S, detail::not_any T, typename... Args>
    sized_any(tnt::size_tag_t<S>, tnt::in_place_tag_t<T>, Args &&...) -> sized_any<S>;

    template <std::size_t S>
    sized_any(sized_any<S> const &) -> sized_any<S>;

    template <std::size_t S>
    sized_any(sized_any<S> &&) -> sized_any<S>;

    template <detail::not_any T, std::size_t S>
    inline T any_cast(sized_any<S> &obj) noexcept(noexcept(obj.template to<T>()))
    {
        return obj.template to<T>();
    }

    template <detail::not_any T, std::size_t S>
    inline T any_cast(sized_any<S> const &obj) noexcept(noexcept(obj.template to<T>()))
    {
        return obj.template to<T>();
    }

    template <detail::not_any T, std::size_t S>
    inline T any_cast(sized_any<S> &&obj) noexcept(noexcept(obj.template to<T>()))
    {
        static_assert(std::is_rvalue_reference_v<T &&> ||
                          std::is_const_v<std::remove_reference_t<T>>,
                      "Can't construct constant reference"
                      "from temporary tnt::sized_any!!");
        return any_cast<T>(obj);
    }

    template <detail::not_any T, typename... Args>
    constexpr auto make_any(Args &&... args) noexcept(
        std::is_nothrow_constructible_v<T, Args...>)
    {
        static_assert(std::constructible_from<T, Args...>,
                      "Inalid arguments passed tnt::make_any!!");
        return sized_any{in_place_t<T>, std::forward<Args>(args)...};
    }

    using any = sized_any<(std::size_t)-1>;
} // namespace tnt

#endif //!TNT_ANY_MIRROR_HPP
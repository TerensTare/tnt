#ifndef TNT_FILE_SNIPPER_HPP
#define TNT_FILE_SNIPPER_HPP

#include <filesystem>

#include "core/Config.hpp"
#include "types/StaticPimpl.hpp"
#include "utils/Traits.hpp"

// TODO:
// make this class's operations asynchronous.
// support watching directories.
// void onModify() specialization.
// arguments for lambda at onModify.
// use vfs alias paths here.

// TODO(maybe):
// a singleton. ??
// std::map<std::string, std::pair<std::string, fs::file_time_type>> instead of tnt::File ??
// move the two concepts to TypeUtils.hpp ??

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace tnt::detail
{
    template <typename T, typename... Args>
    concept callable = std::is_invocable_v<T, Args...>;

    template <typename T, typename... Args>
    concept safe_callable = std::is_nothrow_invocable_v<T, Args...>;
} // namespace tnt::detail
#endif

namespace tnt
{
    /// @brief A basic file watcher. Useful for asset hotloading and other stuff.
    class TNT_API Snipper final
    {
    public:
        /// @brief Watch @c name to check if it has been modified.
        /// @name The file that should be watched.
        void watchFile(std::string_view name);

        /// @brief Call a function if @c file has been modified.
        /// @tparam Func A generic function representation.
        /// @param file The file that should be checked if it is modified or not.
        /// @param func The function that should be called if @c file has been modified.
        /// @note The file watcher silently adds @c file to it's watch
        /// list if the file isn't there already.
        /// @note The function is @c noexcept only if Func() is @c noexcept.
        template <detail::callable<> Func>
        inline void onModify(std::string_view file, Func &&func) noexcept(detail::safe_callable<Func>)
        {
            // TODO: lock
            if (isModified(file))
                func();
        }

        /// @brief Stops watching a file if it was already watching it.
        /// @note If the file wasn't being watched, the file watcher
        /// doesn'r report anything.
        void unwatchFile(std::string_view filename) noexcept;

        /// @brief Check if @c file has been modified since the last check.
        /// @param file The name of the file to check.
        /// @return bool
        bool isModified(std::string_view file) noexcept;

    private:
        struct impl;

        static constexpr std::size_t msvc_size{96};
        static constexpr std::size_t other_size{88};

        std::conditional_t<is_msvc_v,
                           pimpl<impl, msvc_size, 8>,
                           pimpl<impl, other_size, 8>>
            data;
    };
} // namespace tnt

#endif //! TNT_FILE_SNIPPER_HPP
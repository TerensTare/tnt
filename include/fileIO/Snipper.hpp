#ifndef TNT_FILE_SNIPPER_HPP
#define TNT_FILE_SNIPPER_HPP

#include <map>
#include <mutex>
#include <filesystem>

// TODO:
// make this class's operations asynchronous.
// support watching directories.
// void onModify() specialization.
// arguments for lambda at onModify.

// TODO(maybe):
// a singleton. ??
// std::map<std::string, std::pair<std::string, fs::file_time_type>> instead of tnt::File ??
// move the tow concepts to TypeUtils.hpp ??

namespace tnt::detail
{
    template <typename T, typename... Args>
    concept callable = std::is_invocable<T, Args...>::value;

    template <typename T, typename... Args>
    concept safe_callable = std::is_nothrow_invocable<T, Args...>::value;
} // namespace tnt::detail

namespace tnt
{
    using file = std::pair<std::string, std::filesystem::file_time_type>;

    /// @brief A basic file watcher. Useful for asset hotloading and other stuff.
    class Snipper final
    {
    public:
        /// @brief Watch @c name to check if it has been modified.
        /// @name The file that should be watched.
        void watchFile(std::string_view name);

        /// @brief Call a function if @c file has been modified.
        /// @tparam Func A generix function representation.
        /// @param file The file that should be checked if it is modified or not.
        /// @param func The function that should be called if @c file has been modified.
        /// @note The file watcher silently adds @c file to it's watch
        /// list if the file isn't there already.
        /// @note The function is @c noexcept only if Func() is @c noexcept.
        template <typename Func>
        requires detail::callable<Func> auto onModify(std::string_view file, Func &&func) noexcept(std::is_nothrow_invocable_v<Func>)
            -> decltype(func())
        {
            std::lock_guard lock{mtx};
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
        std::map<std::string, file> files;
        mutable std::recursive_mutex mtx;
    };
} // namespace tnt

#endif //! TNT_FILE_SNIPPER_HPP
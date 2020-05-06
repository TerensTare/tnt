#ifndef TNT_FILE_SNIPPER_HPP
#define TNT_FILE_SNIPPER_HPP

#include <map>
#include <mutex>

#include "fileIO/File.hpp"

// TODO:
// make this class's operations asynchronous.
// support watching directories.
// void onModify() specialization.
// arguments for lambda at onModify.

// TODO(maybe):
// a singleton. ??
// final class ??

namespace tnt::file
{
/// @brief A basic file watcher. Useful for asset hotloading and other stuff.
class Snipper
{
public:
  /// @brief The destructor of the file watcher.
  ~Snipper() noexcept;

  /// @brief Watches @em name to check if it has been modified.
  /// @name The file that should be watched.
  void watchFile(std::string_view name);

  /// @brief Calls a function if @em file has been modified.
  /// @tparam Func A generix function representation.
  /// @param file The file that should be checked if it is modified or not.
  /// @param func The function that should be called if @em file has been modified.
  /// @note The file watcher silently adds @em file to it's watch
  /// list if the file isn't there already.
  /// @note The function is @em noexcept only if Func() is @em noexcept.
  template <typename Func, typename = std::enable_if_t<std::is_function_v<Func>, int>>
  auto onModify(std::string_view file, Func func) noexcept(noexcept(func()))
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

private:
  bool isModified(std::string_view file) noexcept;

  std::map<std::string, File *> files;
  static std::recursive_mutex mtx;
};
} // namespace tnt::file

#endif //! TNT_FILE_SNIPPER_HPP
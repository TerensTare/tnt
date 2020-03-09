#ifndef EXIT_QUEUE_HPP
#define EXIT_QUEUE_HPP

// An utility to run several functions if the application needs to quit.

// TODO(maybe):
// Add functions maybe have no arguments.
// this should be an Action ??

namespace tnt
{
class ExitQueue
{
public:
    static ExitQueue &This();

    template <typename... Args>
    void Add(void (*func)(Args &&... args));
};
} // namespace tnt

#endif //!EXIT_QUEUE_HPP
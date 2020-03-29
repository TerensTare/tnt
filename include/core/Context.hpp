#ifndef TNT_CONTEXT_HPP
#define TNT_CONTEXT_HPP

#include "core/Window.hpp"

namespace tnt
{
struct Context
{
    Window *win;
};

Context &defaultCtx;
} // namespace tnt

#endif //!TNT_CONTEXT_HPP
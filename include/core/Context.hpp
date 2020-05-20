#ifndef TNT_CONTEXT_HPP
#define TNT_CONTEXT_HPP

namespace sol
{
    class state;
}

typedef struct lua_State lua_State;

// TODO: add threadsafe logger.

// TODO(maybe):
// add Timer* timer ??

namespace tnt
{
    inline struct Context
    {
        Context() noexcept;

        lua_State *luaState() const noexcept;

    private:
        sol::state *lua_;
    } context{};
} // namespace tnt

#endif //!TNT_CONTEXT_HPP
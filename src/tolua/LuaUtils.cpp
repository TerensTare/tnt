// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <sol/sol.hpp>

#include "doo_ecs/Base.hpp"
#include "tolua/LuaUtils.hpp"

#include "utils/Logger.hpp"
#include "utils/SparseSet.hpp"

namespace sol
{
    template <std::unsigned_integral T>
    struct is_container<tnt::sparse_set<T>> : std::true_type
    {
    };

    template <std::unsigned_integral T>
    struct usertype_container<tnt::sparse_set<T>>
    {
        static int set(lua_State *l)
        {
            auto self{sol::stack::get<tnt::sparse_set<T>>(l, 1)};
            std::size_t const key{sol::stack::get<std::size_t>(l, 2)};

            sol::stack::check<T>(l, 3);

            if (T const &value{sol::stack::get<T>(l, 3)};
                value == sol::nil)
                self.erase(value);
            else if (self.size() == key - 1) // Lua index starts from 1
                self.push_back(value);
            else if (self.size() > key - 1)
                self.edit(key - 1, value);
            else
                self.insert(key - 1, value);

            return 1;
        }

        static tnt::sparse_iterator<T> end(lua_State *l, tnt::sparse_set<T> const &self)
        {
            return tnt::sparse_iterator<T>{self, -1};
        }
    };
} // namespace sol

namespace
{
    template <std::unsigned_integral T>
    inline void loadGenericSparseSet(sol::state_view lua_)
    {
        lua_.new_usertype<tnt::sparse_set<T>>(
            "sparse_set", sol::constructors<tnt::sparse_set<T>()>{},

            "contains", &tnt::sparse_set<T>::contains,
            "index", &tnt::sparse_set<T>::index,
            "empty", &tnt::sparse_set<T>::empty,

            "iterable", [](tnt::sparse_set<T> &s) {
                return sol::as_container(s);
            });
    }
} // namespace

void tnt::lua::loadSparseSet(sol::state_view lua_)
{
    loadGenericSparseSet<tnt::doo::object>(lua_);
}

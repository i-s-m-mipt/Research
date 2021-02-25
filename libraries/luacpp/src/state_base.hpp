#pragma once

#include <cstdarg>
#include <exception>
#include <stdexcept>
#include <string>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "utility.hpp"

namespace lua 
{
    class State_Base 
    {
    public:

        using state_t = lua_State * ;

    public:

        State_Base() : m_state(luaL_newstate()) 
        {}

        State_Base(state_t state) : m_state(state)
        {}

        ~State_Base() noexcept = default;

    public:

        const auto state() const noexcept
        {
            return m_state;
        }

    public:

        auto is_number(int index) const
        {
            return lua_isnumber(m_state, index);
        }

        auto is_string(int index) const
        {
            return lua_isstring(m_state, index);
        }

        auto is_cfunction(int index) const
        {
            return lua_iscfunction(m_state, index);
        }

        auto is_integer(int index) const
        {
            return lua_isinteger(m_state, index);
        }

        auto is_user_data(int index) const
        {
            return lua_isuserdata(m_state, index);
        }

        auto type_code(int index) const
        {
            return lua_type(m_state, index);
        }

        auto type_name(int index) const
        {
            return lua_typename(m_state, type_code(index));
        }

        void register_lua(const char * name, lua_CFunction f) const 
        {
            return lua_register(m_state, name, f);
        }

        void push_nil() const
        {
            lua_pushnil(m_state);
        }

        void push_number(lua_Number number) const
        {
            lua_pushnumber(m_state, number);
        }

        void push_integer(lua_Integer integer) const
        {
            lua_pushinteger(m_state, integer);
        }

        const char * push_lstring(const char * string, std::size_t length) const
        {
            return lua_pushlstring(m_state, string, length);
        }

        const char * push_string(const char * string) const
        {
            return lua_pushstring(m_state, string);
        }

        const char * push_vfstring(const char * format, va_list list) const
        {
            return lua_pushvfstring(m_state, format, list);
        }

        const char * push_fstring(const char * format, ...) const
        {
            va_list list;

            va_start(list, format);

            auto result = lua_pushvfstring(m_state, format, list);

            va_end(list);

            return result;
        }

        void push_closure(lua_CFunction function, int arguments_counter) const
        {
            lua_pushcclosure(m_state, function, arguments_counter);
        }

        void push_function(lua_CFunction function) const
        {
            push_closure(function, 0);
        }

        void push_boolean(int boolean) const
        {
            lua_pushboolean(m_state, boolean);
        }
        
        void push_light_user_data(void * data) const
        {
            lua_pushlightuserdata(m_state, data);
        }

        auto push_thread() const
        {
            return lua_pushthread(m_state);
        }

        auto abs_index(int index) const
        {
            return lua_absindex(m_state, index);
        }

        auto get_top() const
        {
            return lua_gettop(m_state);
        }

        void set_top(int index) const
        {
            lua_settop(m_state, index);
        }

        void push_value(int index) const
        {
            lua_pushvalue(m_state, index);
        }

        void rotate(int index, int rotation) const
        {
            lua_rotate(m_state, index, rotation);
        }

        void copy(int index_from, int index_to) const
        {
            lua_copy(m_state, index_from, index_to);
        }

        auto check_stack(int required_size) const
        {
            return lua_checkstack(m_state, required_size);
        }

        void pop(int n = 1) const
        {
            set_top(-1 * n - 1);
        }

        void insert(int index) const
        {
            rotate(index, 1);
        }

        void remove(int index) const
        {
            rotate(index, -1);
            pop();
        }

        void replace(int index) const
        {
            copy(-1, index);
            pop();
        }

        auto get_global(const char * name) const
        {
            return lua_getglobal(m_state, name);
        }

        auto get_table(int index) const
        {
            return lua_gettable(m_state, index);
        }

        auto get_field(int index, const char * key) const
        {
            return lua_getfield(m_state, index, key);
        }

        auto get_field(int index, lua_Integer position) const
        {
            return lua_geti(m_state, index, position);
        }

        auto raw_get(int index) const
        {
            return lua_rawget(m_state, index);
        }

        auto raw_get_field(int index, lua_Integer position) const
        {
            return lua_rawgeti(m_state, index, position);
        }

        auto raw_get_field(int index, const void * position) const
        {
            return lua_rawgetp(m_state, index, position);
        }

        void create_table(int columns, int rows) const
        {
            lua_createtable(m_state, columns, rows);
        }

        auto new_user_data(std::size_t size) const
        {
            return lua_newuserdata(m_state, size);
        }

        auto get_meta_table(int index) const
        {
            return lua_getmetatable(m_state, index);
        }

        auto get_user_value(int index) const
        {
            return lua_getuservalue(m_state, index);
        }

        void set_global(const char* name) const
        {
            lua_setglobal(m_state, name);
        }

        void set_table(int index) const
        {
            lua_settable(m_state, index);
        }

        void set_field(int index, const char * key) const
        {
            lua_setfield(m_state, index, key);
        }

        void set_field(int index, lua_Integer position) const
        {
            lua_seti(m_state, index, position);
        }
        
        void raw_set(int index) const
        {
            lua_rawset(m_state, index);
        }

        void raw_set_field(int index, lua_Integer position) const
        {
            lua_rawseti(m_state, index, position);
        }

        void raw_set_field(int index, const void * position) const
        {
            lua_rawsetp(m_state, index, position);
        }

        auto set_meta_table(int index) const
        {
            return lua_setmetatable(m_state, index);
        }

        void set_user_value(int index) const
        {
            lua_setuservalue(m_state, index);
        }

        auto is_none(int index) const
        {
            return (type_code(index) == LUA_TNONE); // -1
        }

        auto is_nil(int index) const
        {
            return (type_code(index) == LUA_TNIL); // 0
        }

        auto is_boolean(int index) const
        {
            return (type_code(index) == LUA_TBOOLEAN); // 1
        }

        auto is_light_user_data(int index) const
        {
            return (type_code(index) == LUA_TLIGHTUSERDATA); // 2
        }

        //auto is_number(int index) const
        //{
        //    return (type_code(index) == LUA_TNUMBER); // 3
        //}

        //auto is_string(int index) const
        //{
        //    return (type_code(index) == LUA_TSTRING); // 4
        //}

        auto is_table(int index) const
        {
            return (type_code(index) == LUA_TTABLE); // 5
        }

        auto is_function(int index) const
        {
            return (type_code(index) == LUA_TFUNCTION); // 6
        }
        
        //auto is_user_data(int index) const
        //{
        //    return (type_code(index) == LUA_TUSERDATA); // 7
        //}

        auto is_thread(int index) const
        {
            return (type_code(index) == LUA_TTHREAD); // 8
        }

        auto is_none_or_nil(int index) const
        {
            return (is_none(index) || is_nil(index));
        }

        auto to_number(int index) const
        {
            return lua_tonumberx(m_state, index, nullptr);
        }

        auto to_integer(int index) const
        {
            return lua_tointegerx(m_state, index, nullptr);
        }

        auto to_boolean(int index) const
        {
            return lua_toboolean(m_state, index);
        }

        auto to_string(int index) const
        {
            return lua_tolstring(m_state, index, nullptr);
        }

        auto raw_len(int index) const
        {
            return lua_rawlen(m_state, index);
        }

        auto to_function(int index) const
        {
            return lua_tocfunction(m_state, index);
        }

        auto to_user_data(int index) const
        {
            return lua_touserdata(m_state, index);
        }

        auto to_thread(int index) const
        {
            return lua_tothread(m_state, index);
        }

        auto to_pointer(int index) const
        {
            return lua_topointer(m_state, index);
        }

        void new_table() const
        {
            create_table(0, 0);
        }

        auto next(int index) const
        {
            return lua_next(m_state, index);
        }

        auto pcall(int n_arguments, int n_results, int error_code) const
        {
            return lua_pcallk(m_state, n_arguments, n_results, error_code, 0, nullptr);
        }

    protected:

        state_t m_state;
    };

} // namespace lua
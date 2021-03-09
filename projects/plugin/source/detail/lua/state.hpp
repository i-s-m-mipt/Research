#ifndef SOLUTION_PLUGIN_DETAIL_LUA_STATE_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_STATE_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <cstdarg>
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>

#include <lua.hpp>

#include "../../../../shared/source/logger/logger.hpp"

namespace solution
{
    namespace plugin
    {
        namespace detail
        {
            namespace lua
            {
                class state_exception : public std::exception
                {
                public:

                    explicit state_exception(const std::string & message) noexcept :
                        std::exception(message.c_str())
                    {}

                    explicit state_exception(const char * const message) noexcept :
                        std::exception(message)
                    {}

                    ~state_exception() noexcept = default;
                };

                class State
                {
                public:

                    using state_t = lua_State * ;

                    using boolean_t   = int;
                    using pointer_t   = void * ;
                    using reference_t = int;

                    using number_t    = lua_Number;
                    using integer_t   = lua_Integer;
                    using unsigned_t  = lua_Unsigned;
                    using function_t  = lua_CFunction;
                    using allocator_t = lua_Alloc;
                    using reader_t    = lua_Reader;
                    using writer_t    = lua_Writer;

                public:

                    enum class Type_Code
                    {
                        none = -1,
                        nil,
                        boolean,
                        light_user_data,
                        number,
                        string,
                        table,
                        function,
                        user_data,
                        thread,
                        size
                    };

                public:

                    enum class Operation_Code
                    {
                        add = 0,
                        sub,
                        mul,
                        mod,
                        pow,
                        div,
                        idiv,
                        band,
                        bor,
                        bxor,
                        shl,
                        shr,
                        unm,
                        bnot,
                        eq = 0,
                        lt,
                        le
                    };

                public:

                    enum class Garbage_Collection_Code
                    {
                        stop = 0,
                        restart,
                        collect,
                        count,
                        countb,
                        step,
                        set_pause,
                        set_step_mul,
                        is_running = 9
                    };

                public:

                    explicit State(state_t state = nullptr) :
                        m_state(state ? state : luaL_newstate()) // !
                    {}

                    ~State() noexcept = default;

                public:

                    const auto state() const noexcept
                    {
                        return m_state;
                    }

                public: // state manipulations

                    auto new_state(allocator_t allocator, void * ptr) const
                    {
                        return State(lua_newstate(allocator, ptr));
                    }

                    void close() const
                    {
                        lua_close(m_state);
                    }

                    auto new_thread() const
                    {
                        return State(lua_newthread(m_state));
                    }

                    auto update_panic_function(function_t function) const
                    {
                        return lua_atpanic(m_state, function);
                    }

                    auto version() const
                    {
                        return lua_version(m_state);
                    }

                public: // basic stack manipulations

                    auto abs(int index) const
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

                    void pop(int size = 1) const
                    {
                        set_top(-1 * size - 1);
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

                    void move(state_t state_from, state_t state_to, int size) const
                    {
                        lua_xmove(state_from, state_to, size);
                    }

                public: // access functions

                    auto is_number(int index = -1) const
                    {
                        return lua_isnumber(m_state, index);
                    }

                    auto is_string(int index = -1) const
                    {
                        return lua_isstring(m_state, index);
                    }

                    auto is_cfunction(int index = -1) const
                    {
                        return lua_iscfunction(m_state, index);
                    }

                    auto is_integer(int index = -1) const
                    {
                        return lua_isinteger(m_state, index);
                    }

                    auto is_user_data(int index = -1) const
                    {
                        return lua_isuserdata(m_state, index);
                    }

                    auto type_code(int index = -1) const
                    {
                        return lua_type(m_state, index);
                    }

                    auto type_name(int index = -1) const
                    {
                        return lua_typename(m_state, type_code(index));
                    }

                    auto to_number(int index = -1) const
                    {
                        return lua_tonumberx(m_state, index, nullptr);
                    }

                    auto to_integer(int index = -1) const
                    {
                        return lua_tointegerx(m_state, index, nullptr);
                    }

                    auto to_boolean(int index = -1) const
                    {
                        return static_cast < bool > (lua_toboolean(m_state, index));
                    }

                    auto to_string(int index = -1) const
                    {
                        return std::string(lua_tolstring(m_state, index, nullptr));
                    }

                    auto raw_length(int index = -1) const
                    {
                        return lua_rawlen(m_state, index);
                    }

                    auto to_function(int index = -1) const
                    {
                        return lua_tocfunction(m_state, index);
                    }

                    auto to_user_data(int index = -1) const
                    {
                        return lua_touserdata(m_state, index);
                    }

                    auto to_thread(int index = -1) const
                    {
                        return lua_tothread(m_state, index);
                    }

                    auto to_pointer(int index = -1) const
                    {
                        return lua_topointer(m_state, index);
                    }

                public: // arithmetic functions

                    void apply_operation(Operation_Code operation_code) const
                    {
                        lua_arith(m_state, static_cast < int > (operation_code));
                    }

                    auto raw_equal(int index_lhs, int index_rhs) const
                    {
                        return lua_rawequal(m_state, index_lhs, index_rhs);
                    }

                    auto compare(int index_lhs, int index_rhs, Operation_Code operation_code) const
                    {
                        return lua_compare(m_state, index_lhs, index_rhs, static_cast < int > (operation_code));
                    }

                public: // push functions

                    void push_nil() const
                    {
                        lua_pushnil(m_state);
                    }

                    void push_number(number_t number) const
                    {
                        lua_pushnumber(m_state, number);
                    }

                    void push_integer(integer_t integer) const
                    {
                        lua_pushinteger(m_state, integer);
                    }

                    auto push_lstring(const char * string, std::size_t length) const
                    {
                        return lua_pushlstring(m_state, string, length);
                    }

                    auto push_string(const char * string) const
                    {
                        return lua_pushstring(m_state, string);
                    }

                    auto push_string(const std::string & string) const
                    {
                        return push_string(string.c_str());
                    }

                    auto push_vfstring(const char * format, va_list list) const
                    {
                        return lua_pushvfstring(m_state, format, list);
                    }

                    auto push_fstring(const char * format, ...) const
                    {
                        va_list list;

                        va_start(list, format);

                        auto result = lua_pushvfstring(m_state, format, list);

                        va_end(list);

                        return result;
                    }

                    void push_closure(function_t function, int n_arguments) const
                    {
                        lua_pushcclosure(m_state, function, n_arguments);
                    }

                    void push_function(function_t function) const
                    {
                        push_closure(function, 0);
                    }

                    void push_boolean(bool boolean) const
                    {
                        lua_pushboolean(m_state, static_cast < int > (boolean));
                    }

                    void push_light_user_data(void * data) const
                    {
                        lua_pushlightuserdata(m_state, data);
                    }

                    auto push_thread() const
                    {
                        return lua_pushthread(m_state);
                    }

                    void push_pointer(pointer_t pointer) const
                    {
                        push_light_user_data(pointer);
                    }

                public: // get functions

                    auto get_global(const char * name) const
                    {
                        return lua_getglobal(m_state, name);
                    }

                    auto get_global(const std::string & name) const
                    {
                        return get_global(name.c_str());
                    }

                    auto get_table(int index = -2) const
                    {
                        return lua_gettable(m_state, index);
                    }

                    auto get_field(int index, const char * key) const
                    {
                        return lua_getfield(m_state, index, key);
                    }

                    auto get_field(int index, const std::string & key) const
                    {
                        return get_field(index, key.c_str());
                    }

                    auto get_field(int index, integer_t position) const
                    {
                        return lua_geti(m_state, index, position);
                    }

                    auto raw_get(int index = -2) const
                    {
                        return lua_rawget(m_state, index);
                    }

                    auto raw_get_field(int index, integer_t position) const
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

                    void new_table() const
                    {
                        create_table(0, 0);
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

                public: // set functions

                    void set_global(const char * name) const
                    {
                        lua_setglobal(m_state, name);
                    }

                    void set_global(const std::string & name) const
                    {
                        set_global(name.c_str());
                    }

                    void set_table(int index = -3) const
                    {
                        lua_settable(m_state, index);
                    }

                    void set_field(int index, const char * key) const
                    {
                        lua_setfield(m_state, index, key);
                    }

                    void set_field(int index, const std::string & key) const
                    {
                        set_field(index, key.c_str());
                    }

                    void set_field(int index, integer_t position) const
                    {
                        lua_seti(m_state, index, position);
                    }

                    void raw_set(int index = -3) const
                    {
                        lua_rawset(m_state, index);
                    }

                    void raw_set_field(int index, integer_t position) const
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

                public: // load and call functions

                    auto call(int n_arguments, int n_results = 1) const
                    {
                        return lua_callk(m_state, n_arguments, n_results, 0, nullptr);
                    }

                    auto pcall(int n_arguments, int n_results = 1, int error_code = 0) const
                    {
                        return lua_pcallk(m_state, n_arguments, n_results, error_code, 0, nullptr);
                    }

                    auto load(reader_t reader, void * data, const char * chunk_name, const char * mode) const
                    {
                        return lua_load(m_state, reader, data, chunk_name, mode);
                    }

                    auto dump(writer_t writer, void * data, int strip) const
                    {
                        return lua_dump(m_state, writer, data, strip);
                    }

                public: // coroutine functions

                    auto yield(int n_results = -1) const
                    {
                        return lua_yieldk(m_state, n_results, 0, nullptr);
                    }

                    auto resume(state_t from, int n_arguments) const
                    {
                        return lua_resume(m_state, from, n_arguments);
                    }

                    auto status() const
                    {
                        return lua_status(m_state);
                    }

                    auto is_yieldable() const
                    {
                        return lua_isyieldable(m_state);
                    }

                public: // garbage-collection function

                    auto garbage_collect(int what, int data) const
                    {
                        return lua_gc(m_state, what, data);
                    }

                public: // miscellaneous functions

                    auto error() const
                    {
                        return lua_error(m_state);
                    }

                    auto next(int index) const
                    {
                        return lua_next(m_state, index);
                    }

                    void concat(int size) const
                    {
                        lua_concat(m_state, size);
                    }

                    void length(int index) const
                    {
                        lua_len(m_state, index);
                    }

                    auto string_to_number(const char * string) const
                    {
                        return lua_stringtonumber(m_state, string);
                    }

                    auto string_to_number(const std::string & string) const
                    {
                        return string_to_number(string.c_str());
                    }

                    auto get_allocator(void ** ptr) const
                    {
                        return lua_getallocf(m_state, ptr);
                    }

                    void set_allocator(allocator_t allocator, void * ptr) const
                    {
                        lua_setallocf(m_state, allocator, ptr);
                    }

                public: // additional functions

                    void register_function(const std::string & name, function_t function) const
                    {
                        push_function(function);

                        set_global(name);
                    }

                    auto is_none(int index = -1) const
                    {
                        return (type_code(index) == static_cast < int > (Type_Code::none));
                    }

                    auto is_nil(int index = -1) const
                    {
                        return (type_code(index) == static_cast < int > (Type_Code::nil));
                    }

                    auto is_boolean(int index = -1) const
                    {
                        return (type_code(index) == static_cast < int > (Type_Code::boolean));
                    }

                    auto is_light_user_data(int index = -1) const
                    {
                        return (type_code(index) == static_cast < int > (Type_Code::light_user_data));
                    }

                    /*
                    auto is_number(int index = -1) const
                    {
                        return (type_code(index) == static_cast < int > (Type_Code::number));
                    }
                    */

                    /*
                    auto is_string(int index = -1) const
                    {
                        return (type_code(index) == static_cast < int > (Type_Code::string));
                    }
                    */

                    auto is_table(int index = -1) const
                    {
                        return (type_code(index) == static_cast < int > (Type_Code::table));
                    }

                    auto is_function(int index = -1) const
                    {
                        return (type_code(index) == static_cast < int > (Type_Code::function));
                    }

                    /*
                    auto is_user_data(int index = -1) const
                    {
                        return (type_code(index) == static_cast < int > (Type_Code::user_data));
                    }
                    */

                    auto is_thread(int index = -1) const
                    {
                        return (type_code(index) == static_cast < int > (Type_Code::thread));
                    }

                    auto is_none_or_nil(int index = -1) const
                    {
                        return (is_none(index) || is_nil(index));
                    }

                    auto is_pointer(int index = -1) const
                    {
                        return (
                            is_light_user_data(index) || is_function(index) ||
                            is_cfunction      (index) || is_thread  (index));
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

                private:

                    state_t m_state;
                };

            } // namespace lua

        } // namespace detail

    } // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_STATE_HPP
#ifndef SOLUTION_PLUGIN_DETAIL_LUA_GENERAL_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_GENERAL_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <lua.hpp>

namespace detail
{
    namespace lua
    {
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

        using registration_t = luaL_Reg;

    } // namespace lua 

} // namespace detail

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_GENERAL_HPP
#pragma once

#include <exception>
#include <stdexcept>
#include <string>

#include <lua.hpp>

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

    using boolean_t = int;
    using pointer_t = void * ;

    using number_t    = lua_Number;
    using integer_t   = lua_Integer;
    using unsigned_t  = lua_Unsigned;
    using function_t  = lua_CFunction;
    using allocator_t = lua_Alloc;
    using reader_t    = lua_Reader;
    using writer_t    = lua_Writer;

    using registration_t = luaL_Reg;

} // namespace lua 
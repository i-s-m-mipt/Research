#pragma once

#include <exception>
#include <stdexcept>
#include <string>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

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

    using number_t    = lua_Number;
    using integer_t   = lua_Integer;
    using unsigned_t  = lua_Unsigned;
    using function_t  = lua_CFunction;
    using allocator_t = lua_Alloc;

} // namespace lua 

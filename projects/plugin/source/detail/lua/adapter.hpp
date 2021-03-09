#ifndef SOLUTION_PLUGIN_DETAIL_LUA_ADAPTER_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_ADAPTER_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

namespace solution
{
    namespace plugin
    {
        namespace detail
        {
            namespace lua
            {
                template < typename T >
                struct Adapter;

            } // namespace lua

        } // namespace detail

    } // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_ADAPTER_HPP
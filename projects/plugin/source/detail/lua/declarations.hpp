#ifndef SOLUTION_PLUGIN_DETAIL_LUA_DECLARATIONS_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_DECLARATIONS_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

namespace detail
{
	namespace lua
	{
		class State;

		template < typename T >
		class Entity;

		template < typename T >
		struct Type_Adapter;

	} // namespace lua

} // namespace detail

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_DECLARATIONS_HPP
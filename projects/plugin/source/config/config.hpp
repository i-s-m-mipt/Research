#ifndef SOLUTION_PLUGIN_CONFIG_HPP
#define SOLUTION_PLUGIN_CONFIG_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <exception>
#include <stdexcept>
#include <string>

#include <qluacpp/qlua>

namespace solution
{
	namespace plugin
	{
		struct Config
		{
			using api_t = lua::API;
		};

	} // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_CONFIG_HPP
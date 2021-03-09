#ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_REAL_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_TYPES_REAL_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <exception>
#include <stdexcept>
#include <string>

#include "../adapter.hpp"
#include "../general.hpp"
#include "../state.hpp"

#include "../../../../../shared/source/logger/logger.hpp"

namespace solution
{
    namespace plugin
    {
        namespace detail
        {
            namespace lua
            {
                namespace types
                {
                    class real_exception : public std::exception
                    {
                    public:

                        explicit real_exception(const std::string & message) noexcept :
                            std::exception(message.c_str())
                        {}

                        explicit real_exception(const char * const message) noexcept :
                            std::exception(message)
                        {}

                        ~real_exception() noexcept = default;
                    };

                    template < typename T >
                    class Real
                    {
                    public:

                        using type = T;

                    public:

                        static auto get(State state, int index)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                return static_cast < type > (state.to_number(index));
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < real_exception > (logger, exception);
                            }
                        }

                        static void set(State state, int index, type value)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                state.push_number(static_cast <number_t> (value));

                                if (index != 0)
                                {
                                    state.replace(index - 1);
                                }
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < real_exception > (logger, exception);
                            }
                        }
                    };

                } // namespace types

                template <>
                struct Type_Adapter < float > : public types::Real < float >
                {};

                template <>
                struct Type_Adapter < double > : public types::Real < double >
                {};

            } // namespace lua

        } // namespace detail

    } // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_REAL_HPP
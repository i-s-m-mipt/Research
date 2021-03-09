#ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_BOOLEAN_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_TYPES_BOOLEAN_HPP

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
                    class boolean_exception : public std::exception
                    {
                    public:

                        explicit boolean_exception(const std::string & message) noexcept :
                            std::exception(message.c_str())
                        {}

                        explicit boolean_exception(const char * const message) noexcept :
                            std::exception(message)
                        {}

                        ~boolean_exception() noexcept = default;
                    };

                    class Boolean
                    {
                    public:

                        using type = bool;

                    public:

                        static auto get(State state, int index)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                return static_cast < type > (state.to_boolean(index));
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < boolean_exception > (logger, exception);
                            }
                        }

                        static void set(State state, int index, type value)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                state.push_boolean(static_cast < boolean_t > (value));

                                if (index != 0)
                                {
                                    state.replace(index - 1);
                                }
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < boolean_exception > (logger, exception);
                            }
                        }
                    };

                } // namespace types

                template <>
                struct Type_Adapter < bool > : public types::Boolean
                {};

            } // namespace lua

        } // namespace detail

    } // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_BOOLEAN_HPP
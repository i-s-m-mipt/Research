#ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_POINTER_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_TYPES_POINTER_HPP

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
                    class pointer_exception : public std::exception
                    {
                    public:

                        explicit pointer_exception(const std::string & message) noexcept :
                            std::exception(message.c_str())
                        {}

                        explicit pointer_exception(const char * const message) noexcept :
                            std::exception(message)
                        {}

                        ~pointer_exception() noexcept = default;
                    };

                    template < typename T >
                    class Pointer
                    {
                    public:

                        using type = T;

                    public:

                        static auto get(State state, int index)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                return state.to_pointer(index);
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < pointer_exception > (logger, exception);
                            }
                        }

                        static void set(State state, int index, type value)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                state.push_pointer(value);

                                if (index != 0)
                                {
                                    state.replace(index - 1);
                                }
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < pointer_exception > (logger, exception);
                            }
                        }
                    };

                } // namespace types

                template <>
                struct Type_Adapter < void * > : public types::Pointer < void * >
                {};

                template <>
                struct Type_Adapter < const void * > : public types::Pointer < const void * >
                {};

            } // namespace lua

        } // namespace detail

    } // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_POINTER_HPP
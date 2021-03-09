#ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_INTEGER_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_TYPES_INTEGER_HPP

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
                    class integer_exception : public std::exception
                    {
                    public:

                        explicit integer_exception(const std::string & message) noexcept :
                            std::exception(message.c_str())
                        {}

                        explicit integer_exception(const char * const message) noexcept :
                            std::exception(message)
                        {}

                        ~integer_exception() noexcept = default;
                    };

                    template < typename T >
                    class Integer
                    {
                    public:

                        using type = T;

                    public:

                        static auto get(State state, int index)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                return static_cast < type > (state.to_integer(index));
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < integer_exception > (logger, exception);
                            }
                        }

                        static void set(State state, int index, type value)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                state.push_integer(static_cast < integer_t > (value));

                                if (index != 0)
                                {
                                    state.replace(index - 1);
                                }
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < integer_exception > (logger, exception);
                            }
                        }
                    };

                } // namespace types

                template <>
                struct Type_Adapter < int > : public types::Integer < int >
                {};

                template <>
                struct Type_Adapter < unsigned int > : public types::Integer < unsigned int >
                {};

                template <>
                struct Type_Adapter < short > : public types::Integer < short >
                {};

                template <>
                struct Type_Adapter < unsigned short > : public types::Integer < unsigned short >
                {};

                template <>
                struct Type_Adapter < long > : public types::Integer < long >
                {};

                template <>
                struct Type_Adapter < unsigned long > : public types::Integer < unsigned long >
                {};

                template <>
                struct Type_Adapter < long long > : public types::Integer < long long >
                {};

                template <>
                struct Type_Adapter < unsigned long long > : public types::Integer < unsigned long long >
                {};

            } // namespace lua

        } // namespace detail

    } // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_INTEGER_HPP
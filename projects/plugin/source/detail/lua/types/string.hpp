#ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_STRING_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_TYPES_STRING_HPP

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
                    class string_exception : public std::exception
                    {
                    public:

                        explicit string_exception(const std::string & message) noexcept :
                            std::exception(message.c_str())
                        {}

                        explicit string_exception(const char * const message) noexcept :
                            std::exception(message)
                        {}

                        ~string_exception() noexcept = default;
                    };

                    class String
                    {
                    public:

                        using type = std::string;

                    public:

                        static auto get(State state, int index)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                auto string = state.to_string(index);

                                if (string == nullptr)
                                {
                                    return type(); // ?
                                }
                                else
                                {
                                    return type(string);
                                }
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < string_exception > (logger, exception);
                            }
                        }

                        static void set(State state, int index, type value)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                state.push_string(value);

                                if (index != 0)
                                {
                                    state.replace(index - 1);
                                }
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < string_exception > (logger, exception);
                            }
                        }
                    };

                } // namespace types 

                template <>
                struct Type_Adapter < std::string > : public types::String
                {};

            } // namespace lua

        } // namespace detail

    } // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_STRING_HPP
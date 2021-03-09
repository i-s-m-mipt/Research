#ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_TABLE_FIELD_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_TYPES_TABLE_FIELD_HPP

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
                    class table_field_exception: public std::exception
                    {
                    public:

                        explicit table_field_exception(const std::string & message) noexcept :
                            std::exception(message.c_str())
                        {}

                        explicit table_field_exception(const char * const message) noexcept :
                            std::exception(message)
                        {}

                        ~table_field_exception() noexcept = default;
                    };

                    template < typename K, typename V >
                    class Table_Field
                    {
                    public:

                        using type = V;

                        using key_t   = K;
                        using value_t = V;

                    public:

                        static auto get(State state, int index, key_t key)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                state.push(key);

                                if (index <= 0)
                                {
                                    state.get_table(index - 1);
                                }
                                else
                                {
                                    state.get_table(index);
                                }

                                auto result = Entity < Type_Adapter < value_t > > (state, -1).get();

                                state.pop();

                                return result;
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < table_field_exception > (logger, exception);
                            }
                        }

                        static void set(State state, int index, value_t value, key_t key)
                        {
                            RUN_LOGGER(logger);

                            try
                            {
                                state.push(key);
                                state.push(value);

                                if (index <= 0)
                                {
                                    state.set_table(index - 2);
                                }
                                else
                                {
                                    state.set_table(index);
                                }
                            }
                            catch (const std::exception & exception)
                            {
                                shared::catch_handler < table_field_exception > (logger, exception);
                            }
                        }
                    };

                } // namespace types

            } // namespace lua

        } // namespace detail

    } // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_TYPES_TABLE_FIELD_HPP
#ifndef SOLUTION_PLUGIN_DETAIL_LUA_ENTITY_HPP
#define SOLUTION_PLUGIN_DETAIL_LUA_ENTITY_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <exception>
#include <stdexcept>
#include <string>

#include "declarations.hpp"

#include "../../../../shared/source/logger/logger.hpp"

namespace detail
{
    namespace lua
    {
        class entity_exception : public std::exception
        {
        public:

            explicit entity_exception(const std::string & message) noexcept :
                std::exception(message.c_str())
            {}

            explicit entity_exception(const char * const message) noexcept :
                std::exception(message)
            {}

            ~entity_exception() noexcept = default;
        };

        template < typename Type_Adapter >
        class Entity
        {
        public:

            using type = typename Type_Adapter::type;

        public:

            explicit Entity(State state, int index) :
                m_state(state), m_index(index)
            {}

            ~Entity() noexcept = default;

        public:

            const auto state() const noexcept
            {
                return m_state;
            }

            const auto index() const noexcept
            {
                return m_index;
            }

        public:

            template < typename ... Types >
            auto get(Types ... args) const
            {
                RUN_LOGGER(logger);

                try
                {
                    return Type_Adapter::get(m_state, m_index, args...);
                }
                catch (const std::exception & exception)
                {
                    shared::catch_handler < entity_exception > (logger, exception);
                }
            }

            template < typename ... Types >
            void set(type value, Types ... args) const
            {
                RUN_LOGGER(logger);

                try
                {
                    Type_Adapter::set(m_state, m_index, value, args...);
                }
                catch (const std::exception & exception)
                {
                    shared::catch_handler < entity_exception > (logger, exception);
                }
            }

        private:

            State m_state;
            int   m_index;
        };

    } // namespace lua

} // namespace detail

#endif // #ifndef SOLUTION_PLUGIN_DETAIL_LUA_ENTITY_HPP
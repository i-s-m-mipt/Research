#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "declarations.hpp"

namespace lua
{
    template < typename T >
    class Entity 
    {
    public:

        using type_adapter_t = T;

        using type = typename type_adapter_t::type;

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

        auto & operator=(type value) const
        {
            set(value);

            return (*this);
        }

    public:

        inline auto type_matches() const 
        {
            return type_adapter_t::type_matches(m_state, m_index);
        }

        template < typename ... Types >
        inline auto get(Types ... args) const 
        {
            return type_adapter_t::get(m_state, m_index, args...);
        }

        template < typename ... Types >
        inline void set(type value, Types ... args) const
        {
            type_adapter_t::set(m_state, m_index, value, args...);
        }

        template < typename F, typename ... Types >
        inline void apply(F function, Types ... args) const
        {
            type_adapter_t::apply(m_state, m_index, function, args...);
        }

    protected: // TODO

        State m_state;
        int   m_index;
    };

} // namespace lua
#pragma once

#include <exception>
#include <stdexcept>
#include <string>

#include "declarations.hpp"

namespace lua 
{
    template < typename T >
    class Element 
    {
    public:

        using type = Element < T > ;

        using value_t = T;

    public:

        explicit Element(State state, int index, int position) :
            m_state(state), m_index(index), m_position(position)
        {}

        ~Element() noexcept = default;

    public:

        void swap(Element & other) noexcept
        {
            std::swap(m_state,    other.m_state);
            std::swap(m_index,    other.m_index);
            std::swap(m_position, other.m_position);
        }

    public:

        auto & operator=(value_t value) const
        {
            set(value);

            return (*this);
        }

    public:

        auto get() const 
        {
            m_state.raw_get_field(m_index, m_position);

            auto result = Entity < Type_Adapter < value_t > > (m_state, -1).get();

            m_state.pop();

            return result;
        }

        void set(value_t value) const
        {
            m_state.push(value);

            m_state.raw_set_field(-2, m_position);
        }

        template < typename F >
        void apply(F function) const 
        {
            m_state.raw_get_field(m_index, m_position);

            function(m_state, -1);

            m_state.pop();
        }

    private:

        State m_state;
        int   m_index;
        int   m_position;
    };

    template < typename T >
    void swap(Element < T > & lhs, Element < T > & rhs) noexcept
    {
        lhs.swap(rhs);
    }

    // =========================================================================

    template < typename T >
    class Vector 
    {
    public:

        using type = Vector < T > ;

        using value_t = T;
        using element_t = Element < T > ;

    public:

        explicit Vector(State state, int index) :
            m_state(state), m_index(index), m_size(0)
        {
            if (m_state.is_nil(index)) 
            {
                m_state.new_table();
                m_state.replace(index - 1);
            }
        }

        ~Vector() noexcept 
        {
            try
            {
                m_state.pop(m_size);
            }
            catch (...)
            {
                // std::abort();
            }
        }

    public:

        auto size() const 
        {
            return m_state.raw_length(m_index);
        }

        auto operator[](int position) const
        {
            return element_t(m_state, m_index, position + 1);
        }

        void push_back(value_t value) const 
        {
            auto position = m_state.raw_length(m_index) + 1;

            m_state.push(value);

            m_state.raw_set_field(m_index - 1, position);
        }

        void pop() const 
        {
            int position = m_state.raw_length(m_index);

            m_state.push_nil();

            m_state.raw_set_field(m_index - 1, position);
        }

        void clear_stack() const
        {
            m_state.pop(m_size);

            m_size = 0;
        }

    private:

        State m_state;
        int   m_index;

    private:

        mutable int m_size;
    };

} // namespace lua
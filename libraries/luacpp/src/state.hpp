#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>

#include "declarations.hpp"
#include "state_base.hpp"

namespace lua 
{
    template < typename ... Types >
    struct tuple_tail;

    template < typename T >
    struct tuple_tail < std::tuple < T > >
    {
        using type = std::tuple <>;
    };

    template < typename T, typename ... Types >
    struct tuple_tail < std::tuple < T, Types ... > >
    {
        using type = std::tuple < Types ... >;
    };

    template < typename T >
    using tuple_tail_t = typename tuple_tail < T > ::type;

    class State : public State_Base 
    {
    public:

        explicit State(state_t state = nullptr) : State_Base(state)
        {}

        ~State() noexcept = default;

    public:

        template < typename T >
        auto at(int index) const 
        {
            return Entity < Type_Adapter < T > > (*this, index);
        }

        template < typename T >
        void push(T value) const 
        {
            at < T > (0) = value;
        }

    public:

        template < typename T >
        void push_values(T value) const
        {
            push(value);
        }

        template < typename T, typename ... Types >
        void push_values(T value, Types ... values) const 
        {
            push(value);
            push_values(values...);
        }

    public:

        template < std::size_t Index = 0, typename ... Types >
        std::enable_if_t < Index == sizeof...(Types), void >
            push_tuple(const std::tuple < Types ... > & tuple) const
        {}

        template < std::size_t Index = 0, typename ... Types >
        std::enable_if_t < Index < sizeof...(Types), void >
            push_tuple(const std::tuple < Types ... > & tuple) const
        {
            push(std::get < Index > (tuple));
            push_tuple < Index + 1, Types ... > (tuple);
        }

    public: // TODO : tuple -> variadic

        template < std::size_t Index = 0, typename T >
        std::enable_if_t < std::tuple_size_v < T > == 0, T >
            get(int index = -1) const 
        {
            return std::make_tuple();
        }

        template < std::size_t Index = 0, typename T >
        std::enable_if_t < std::tuple_size_v < T > != 0, T >
            get(int index = -1) const 
        {
            using value_t = std::tuple_element_t < 0, T > ;

            return std::tuple_cat(std::make_tuple(at < value_t > (index - Index).get()),
                get < Index + 1, tuple_tail_t < T > > (index));
        }

    public: // TODO : tuple -> variadic

        template < typename T, std::size_t Index = std::tuple_size_v < T > >
        std::enable_if_t < std::tuple_size_v < T > == 0, T >
            get_reversed(int index = -1) const 
        {
            return std::make_tuple();
        }

        template < typename T, std::size_t Index = std::tuple_size_v < T > >
        std::enable_if_t < std::tuple_size_v < T > != 0, T >
            get_reversed(int index = -1) const 
        {
            using value_t = std::tuple_element_t < 0, T > ;

            return std::tuple_cat(std::make_tuple(at < value_t > (index - Index + 1).get()), 
                get_reversed < tuple_tail_t < T >, Index - 1 > (index));
        }

    public:

        template < typename callback_t, typename... Types >
        void call_and_apply(callback_t function, int n_results, const char * name, Types ... args) const 
        {
            get_global(name);

            if constexpr (sizeof...(args) > 0)
            {
                push_values(args...);
            }

            pcall(sizeof...(args), n_results, 0);

            pop(function(*this));
        }

        template < typename Tuple, typename ... Types >
        auto call(const char * name, Types ... args) const 
        {
            get_global(name);

            if constexpr (sizeof...(args) > 0)
            {
                push_values(args...);
            }

            auto size = std::tuple_size_v < Tuple >;

            pcall(sizeof...(args), size, 0);
           
            auto result = get_reversed < Tuple > ();

            pop(size);

            return result;
        }

        template < typename ... Types >
        auto call_with_results_on_stack(const char * name, int n_results, Types ... args) const 
        {
            get_global(name);

            if constexpr (sizeof...(args) > 0)
            {
                push_values(args...);
            }

            pcall(sizeof...(args), n_results, 0);

            return n_results;
        }
    };

} // namespace lua
#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "../declarations.hpp"
#include "../general.hpp"

namespace lua 
{
    namespace types 
    {
        template < typename T >
        class Integer
        {
        public:

            using type = T;

        public:

            static auto type_matches(State state, int index) 
            {
                return state.is_integer(index); // TODO
            }

            static auto get(State state, int index) 
            {
                return static_cast < T > (state.to_integer(index)); // TODO
            }

            static void set(State state, int index, T value)
            {
                state.push_integer(static_cast < integer_t > (value)); // TODO

                if (index != 0)
                {
                    state.replace(index - 1);
                }
            }

            template < typename F >
            static void apply(State state, int index, F function)
            {
                function(state, index);
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
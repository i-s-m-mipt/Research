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
        class Real 
        {
        public:

            using type = T;

        public:

            static auto type_matches(State state, int index) 
            {
                return state.is_number(index);
            }

            static auto get(State state, int index)
            {
                return static_cast < T > (state.to_number(index));
            }

            static void set(State state, int index, T value)
            {
                state.push_number(static_cast < number_t > (value));

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
    struct Type_Adapter < float > : public types::Real < float >
    {};

    template <>
    struct Type_Adapter < double > : public types::Real < double >
    {};

} // namespace lua
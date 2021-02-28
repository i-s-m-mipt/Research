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
        class Pointer 
        {
        public:

            using type = T;

        public:

            static auto type_matches(State state, int index) 
            {
                return state.is_pointer(index);
            }

            static auto get(State state, int index)
            {
                return state.to_pointer(index); // TODO
            }

            static void set(State state, int index, T value)
            {
                state.push_pointer(value);

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
    struct Type_Adapter < void * > : public types::Pointer < void * >
    {};

    template <>
    struct Type_Adapter < const void * > : public types::Pointer < const void * >
    {};

} // namespace lua
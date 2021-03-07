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
        class Boolean 
        {
        public:

            using type = bool;

        public:

            static auto type_matches(State state, int index) 
            {
                return state.is_boolean(index);
            }

            static auto get(State state, int index)
            {
                return static_cast < bool > (state.to_boolean(index));
            }

            static void set(State state, int index, bool value)
            {
                state.push_boolean(static_cast < boolean_t > (value));

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
    struct Type_Adapter < bool > : public types::Boolean
    {};

} // namespace lua
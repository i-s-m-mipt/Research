#pragma once

#include <exception>
#include <optional>
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
        class Optional 
        {
        public:

            using type = std::optional < T > ;
            
            using type_adapter_t = Type_Adapter < T > ;

        public:

            static auto type_matches(State state, int index) 
            {
                return (state.is_nil(index) || type_adapter_t::type_matches(state, index));
            }

            static auto get(State state, int index)
            {
                if (state.is_nil(index))
                {
                    return std::optional < T > ();
                }
                else
                {
                    return std::optional < T > (type_adapter_t::get(state, index));
                }
            }

            static void set(State state, int index, type value) 
            {
                if (value) 
                {
                    type_adapter_t::set(state, index, *value);
                }
                else 
                {
                    state.push_nil();

                    if (index != 0)
                    {
                        state.replace(index - 1);
                    }
                }
            }

            template < typename F >
            static void apply(State state, int index, F function) 
            {
                type_adapter_t::apply(state, index, function);
            }
        };

    } // namespace types

} // namespace lua 
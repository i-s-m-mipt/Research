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
        template < typename K, typename V >
        class Table_Field 
        {
        public:

            using type = V;

            using key_t   = K;
            using value_t = V;

        public:

            static auto type_matches(State state, int index) 
            {
                return state.is_table(index);
            }

            static auto get(State state, int index, key_t key)
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

            static void set(State state, int index, value_t value, key_t key)
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

            template < typename F >
            static void apply(State state, int index, F function, key_t key)
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
                    
                function(state, index);

                state.pop();
            }
        };

    } // namespace types

} // namespace lua
#pragma once

#include <exception>
#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "../declarations.hpp"
#include "../general.hpp"

namespace lua 
{
    template < typename T >
    class Type_Adapter < Element < T > > 
    {
    public:

        using type = T;

    public:

        static auto type_matches(State state, int index) 
        {
            return state.is_table(index);
        }

        static auto get(State state, int index, int position)
        {
            state.raw_get_field(index, position);

            auto result = Entity < Type_Adapter < T > > (state, -1).get();

            state.pop();

            return result;
        }

        static void set(State state, int index, T value, int position) 
        {
            state.push(position);

            state.push(value);

            state.set_table(index - 2);
        }

        template < typename F >
        static void apply(State state, int index, F function, int position) 
        {
            state.raw_get_field(index, position);

            function(state, index);

            state.pop();
        }
    };

    template < typename T >
    class Type_Adapter < std::vector < T, std::allocator < T > > > // TODO
    {
    public:

        using type = std::vector < T, std::allocator < T > > ;

    public:

        static auto type_matches(State state, int index) 
        {
            return (state.is_nil(index) || state.is_table(index));
        }

        static auto get(State state, int index) 
        {
            return Vector < T > (state, index).get(); // TODO
        }

        static void set(State state, int index, type value)
        {
            // TODO
        }

        template < typename F >
        static void apply(State state, int index, F function) 
        {
            function(state, index);
        }
    };

    template < typename T >
    class Type_Adapter < Vector < T > > 
    {
    public:

        using type = Vector < T > ;

    public:

        static auto type_matches(State state, int index) 
        {
            return (state.is_nil(index) || state.is_table(index));
        }

        static auto get(State state, int index)
        {
            return Vector < T > (state, index);
        }

        static void set(State state, int index, type value)
        {
            // TODO
        }

        template < typename F >
        static void apply(State state, int index, F function) 
        {
            function(state, index);
        }
    };

} // namespace lua
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
        class String_C 
        {
        public:

            using type = T;

        public:

            static auto type_matches(State state, int index) 
            {
                return state.is_string(index);
            }

            static auto get(State state, int index) 
            {
                return state.to_string(index);
            }

            static void set(State state, int index, T value) 
            {
                state.push_string(value);

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

        class String
        {
        public:

            using type = std::string;

        public:

            static auto type_matches(State state, int index) 
            {
                return state.is_string(index);
            }

            static auto get(State state, int index) 
            {
                auto string = state.to_string(index);

                if (string == nullptr)
                {
                    return std::string(); // ?
                }
                else
                {
                    return std::string(string);
                }
            }

            static void set(State state, int index, std::string value)
            {
                state.push_string(value.c_str());

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
    struct Type_Adapter< char * > : public types::String_C < char * > 
    {};

    template <>
    struct Type_Adapter < const char * > : public types::String_C < const char * > 
    {};

    template < std::size_t N >
    struct Type_Adapter < char[N] > : public types::String_C < char * > 
    {};

    template < std::size_t N >
    struct Type_Adapter < const char[N] > : public types::String_C < char * > 
    {};

    template < std::size_t N >
    struct Type_Adapter < const char(&)[N] > : public types::String_C < const char * > 
    {};

    template <>
    struct Type_Adapter < std::string > : public types::String
    {};

} // namespace lua
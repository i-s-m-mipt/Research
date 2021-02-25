#pragma once

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace lua 
{
    template < typename ... Types >
    struct tuple_tail;

    template < typename T >
    struct tuple_tail < std::tuple < T > >
    {
        using type = std::tuple <> ;
    };

    template < typename T, typename ... Types >
    struct tuple_tail < std::tuple < T, Types ... > >
    {
        using type = std::tuple < Types ... > ;
    };

    template < typename T >
    using tuple_tail_t = typename tuple_tail < T > ::type;

    template < std::size_t N >
    struct Apply
    {
        template < typename F, typename T, typename ... Args >
        static inline decltype(auto) apply_tuple(F && f, T && t, Args && ... args)
        {
            return Apply < N - 1U > ::apply_tuple(std::forward < F > (f), std::forward < T > (t),
                std::get < N - 1U > (std::forward < T > (t)), std::forward < Args > (args) ...);
        }
    };

    template <>
    struct Apply < 0U >
    {
        template < typename F, typename T, typename ... Args >
        static inline decltype(auto) apply_tuple(F && f, T &&, Args && ... args)
        {
            return std::forward < F > (f)(std::forward < Args > (args) ...);
        }
    };

    template < typename F, typename T >
    inline decltype(auto) apply_tuple(F && f, T && t)
    {
        return Apply < std::tuple_size_v < std::decay_t < T > > > ::apply_tuple(
            std::forward < F > (f), std::forward < T > (t));
    }

} // namespace lua 

#ifndef SOLUTION_SYSTEM_DETAIL_JULIA_STATE_HPP
#define SOLUTION_SYSTEM_DETAIL_JULIA_STATE_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <exception>
#include <stdexcept>
#include <string>

#include "../../config/config.hpp" // !

#include <julia.h>

#include "../../../../shared/source/logger/logger.hpp"

namespace solution
{
    namespace system
    {
        namespace detail
        {
            namespace julia
            {
                class state_exception : public std::exception
                {
                public:

                    explicit state_exception(const std::string & message) noexcept :
                        std::exception(message.c_str())
                    {}

                    explicit state_exception(const char * const message) noexcept :
                        std::exception(message)
                    {}

                    ~state_exception() noexcept = default;
                };

                class State {};

            } // namespace julia

        } // namespace detail

    } // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_DETAIL_JULIA_STATE_HPP
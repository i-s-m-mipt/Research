#ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_HPP
#define SOLUTION_SYSTEM_MARKET_OSCILLATORS_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
#include <climits>
#include <cmath>
#include <exception>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "../candle/candle.hpp"

#include "../../../../shared/source/logger/logger.hpp"

#if defined(max)
#  undef max
#endif

#if defined(min)
#  undef min
#endif

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				class oscillator_exception : public std::exception
				{
				public:

					explicit oscillator_exception(const std::string & message) noexcept :
						std::exception(message.c_str())
					{}

					explicit oscillator_exception(const char * const message) noexcept :
						std::exception(message)
					{}

					~oscillator_exception() noexcept = default;
				};

				using candles_container_t = std::vector < Candle > ;

			} // namespace oscillators

		} // namespace market
		
	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_HPP
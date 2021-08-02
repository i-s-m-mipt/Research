#ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_MACD_HPP
#define SOLUTION_SYSTEM_MARKET_OSCILLATORS_MACD_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include "../detail.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				class MACD
				{
				public:

					explicit MACD(std::size_t timesteps_long, std::size_t timesteps_short, std::size_t timesteps_signal) : 
						m_timesteps_long(timesteps_long), m_timesteps_short(timesteps_short), m_timesteps_signal(timesteps_signal)
					{
						initialize();
					}

				private:

					void initialize() const;

				public:

					void operator()(candles_container_t & candles) const;

				private:

					std::size_t m_timesteps_long;
					std::size_t m_timesteps_short;
					std::size_t m_timesteps_signal;
				};

			} // namespace oscillators

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_MACD_HPP
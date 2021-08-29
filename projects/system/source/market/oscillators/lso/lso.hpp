#ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_LSO_HPP
#define SOLUTION_SYSTEM_MARKET_OSCILLATORS_LSO_HPP

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
				class LSO
				{
				public:

					explicit LSO(std::size_t timesteps_fast, std::size_t timesteps_slow) : 
						m_timesteps_fast(timesteps_fast), m_timesteps_slow(timesteps_slow)
					{
						initialize();
					}

				private:

					void initialize() const;

				public:

					void operator()(candles_container_t & candles) const;

				private:

					static inline const double max_value = 100.0;

				private:

					std::size_t m_timesteps_fast;
					std::size_t m_timesteps_slow;
				};

			} // namespace oscillators

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_LSO_HPP
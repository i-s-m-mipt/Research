#ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_BOLLINGER_BANDS_HPP
#define SOLUTION_SYSTEM_MARKET_INDICATORS_BOLLINGER_BANDS_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include "../indicators.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				class BOLLINGER_BANDS
				{
				public:

					explicit BOLLINGER_BANDS(std::size_t timesteps, double deviations) : 
						m_timesteps(timesteps), m_deviations(deviations)
					{
						initialize();
					}

				private:

					void initialize() const;

				public:

					void operator()(candles_container_t & candles) const;

				private:

					std::size_t m_timesteps;

					double m_deviations;
				};

			} // namespace indicators

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_BOLLINGER_BANDS_HPP
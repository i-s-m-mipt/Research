#ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_AMA_HPP
#define SOLUTION_SYSTEM_MARKET_INDICATORS_AMA_HPP

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
				class AMA
				{
				public:

					explicit AMA(std::size_t timesteps, std::size_t fastest, std::size_t slowest) : 
						m_timesteps(timesteps), m_fastest(2.0 / (fastest + 1.0)), m_slowest(2.0 / (slowest + 1.0))
					{
						initialize();
					}

				private:

					void initialize() const;

				public:

					void operator()(candles_container_t & candles) const;

				private:

					std::size_t m_timesteps;

					double m_fastest;
					double m_slowest;
				};

			} // namespace indicators

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_AMA_HPP
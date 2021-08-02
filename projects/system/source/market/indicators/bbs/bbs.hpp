#ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_BBS_HPP
#define SOLUTION_SYSTEM_MARKET_INDICATORS_BBS_HPP

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
			namespace indicators
			{
				class BBS
				{
				public:

					explicit BBS(std::size_t timesteps, double deviations) : 
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

#endif // #ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_BBS_HPP
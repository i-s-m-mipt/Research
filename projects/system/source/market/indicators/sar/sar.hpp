#ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_SAR_HPP
#define SOLUTION_SYSTEM_MARKET_INDICATORS_SAR_HPP

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
				class SAR
				{
				public:

					explicit SAR(std::size_t initial_candles, double min_acceleration, double max_acceleration, double acceleration_step) :
						m_initial_candles(initial_candles), m_min_acceleration(min_acceleration),
						m_max_acceleration(m_max_acceleration), m_acceleration_step(acceleration_step)
					{
						initialize();
					}

				private:

					void initialize() const;

				public:

					void operator()(candles_container_t& candles) const;

				private:

					std::size_t m_initial_candles;
					double m_min_acceleration;
					double m_max_acceleration;
					double m_acceleration_step;
				};

			} // namespace indicators

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_INDICATORS_SAR_HPP
#ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_RVI_HPP
#define SOLUTION_SYSTEM_MARKET_OSCILLATORS_RVI_HPP

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
				class RVI
				{
				public:

					explicit RVI(std::size_t timesteps_sma, std::size_t timesteps_wma) : 
						m_timesteps_sma(timesteps_sma), m_timesteps_wma(timesteps_wma)
					{
						initialize();
					}

				private:

					void initialize() const;

				public:

					void operator()(candles_container_t & candles) const;

				private:

					static inline const double max_value = 0.50;

				private:

					std::size_t m_timesteps_sma;
					std::size_t m_timesteps_wma;
				};

			} // namespace oscillators

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_RVI_HPP
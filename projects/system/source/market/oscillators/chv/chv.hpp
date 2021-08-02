#ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_CHV_HPP
#define SOLUTION_SYSTEM_MARKET_OSCILLATORS_CHV_HPP

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
				class CHV
				{
				public:

					explicit CHV(std::size_t timesteps_ema, std::size_t timesteps_roc) : 
						m_timesteps_ema(timesteps_ema), m_timesteps_roc(timesteps_roc)
					{
						initialize();
					}

				private:

					void initialize() const;

				public:

					void operator()(candles_container_t & candles) const;

				private:

					std::size_t m_timesteps_ema;
					std::size_t m_timesteps_roc;
				};

			} // namespace oscillators

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_CHV_HPP
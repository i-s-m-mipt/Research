#ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_CC_HPP
#define SOLUTION_SYSTEM_MARKET_OSCILLATORS_CC_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include "../oscillators.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				class CC
				{
				public:

					explicit CC(std::size_t timesteps, std::size_t timesteps_long, std::size_t timesteps_short) :
						m_timesteps(timesteps), m_timesteps_long(timesteps_long), m_timesteps_short(timesteps_short)
					{
						initialize();
					}

				private:

					void initialize() const;

				public:

					void operator()(candles_container_t & candles) const;

				private:

					std::size_t m_timesteps;

					std::size_t m_timesteps_long;
					std::size_t m_timesteps_short;
				};

			} // namespace oscillators

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_OSCILLATORS_CC_HPP